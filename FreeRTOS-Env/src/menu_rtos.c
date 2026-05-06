/*
 * menu_rtos_adaptado.c
 *
 * Adaptação RTOS da lógica antiga do Menu.c.
 *
 * Ideia principal:
 *  - O menu deixa de chamar NAVBTN_Pressed() diretamente.
 *  - O menu deixa de escrever diretamente no LCD.
 *  - A task do menu consome eventos de BUTTON_Pressed().
 *  - A task do menu envia comandos para DISPLAY_Send()/DISPLAY_Printf().
 *  - As alterações ao rádio passam por RADIO_RTOS_SET_VOLUME/CHANNEL().
 *  - O relógio é lido/alterado via CLOCK_*().
 */

#include "menu_rtos.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "button.h"
#include "clock.h"
#include "display.h"
#include "radio_rtos.h"
#include "time_sync.h"
#include "Nav7Btn.h"
#include "LCD.h"

#ifndef MENU_REFRESH_MS
#define MENU_REFRESH_MS             250U
#endif

#ifndef MENU_BUTTON_DELAY_MS
#define MENU_BUTTON_DELAY_MS        50U
#endif

#ifndef MENU_MAX_VOLUME
#define MENU_MAX_VOLUME             15U
#endif

#ifndef MENU_MIN_CHANNEL
#define MENU_MIN_CHANNEL            0U       /* 87.0 MHz */
#endif

#ifndef MENU_MAX_CHANNEL
#define MENU_MAX_CHANNEL            210U     /* 108.0 MHz, spacing 100 kHz */
#endif

#ifndef MENU_START_CHANNEL
#define MENU_START_CHANNEL          46U      /* 91.6 MHz */
#endif

#ifndef MENU_START_VOLUME
#define MENU_START_VOLUME           8U
#endif

#define WIFI_MENU_SSID              "iPhone de Rafael"
#define WIFI_MENU_PASSWORD          "12332123"

typedef enum {
    MENU_SCREEN_MAIN = 0,
    MENU_SCREEN_OPTIONS,
    MENU_SCREEN_CONFIRM_DEL_FREQ,
    MENU_SCREEN_CONFIRM_DEL_VOL,
    MENU_SCREEN_SET_CLOCK,
    MENU_SCREEN_SET_DATE
} MenuScreen;

typedef enum {
    MENU_OPTION_CLOCK = 0,
    MENU_OPTION_DATE,
    MENU_OPTION_NTP_SYNC,
    MENU_OPTION_SAVE,
    MENU_OPTION_DEL_FREQ,
    MENU_OPTION_DEL_VOL,
    MENU_OPTION_COUNT
} MenuOption;

static TaskHandle_t menuTaskHandle = NULL;
static SemaphoreHandle_t menuDataMutex = NULL;
static Radio_Data menuRadioData;
static MenuScreen currentScreen = MENU_SCREEN_MAIN;
static MenuOption currentOption = MENU_OPTION_CLOCK;
static uint8_t confirmSelection = 0; /* 0 = Yes, 1 = No */
static tm editDateTime;
static uint8_t editField = 0;
static uint8_t initialized = 0;

static void MENU_Lock(void) {
    if (menuDataMutex != NULL) {
        xSemaphoreTake(menuDataMutex, portMAX_DELAY);
    }
}

static void MENU_Unlock(void) {
    if (menuDataMutex != NULL) {
        xSemaphoreGive(menuDataMutex);
    }
}

static void format_frequency(char *out, size_t out_size, uint16_t channel, uint8_t volume) {
    /*
     * Evita usar %.1f porque em muitos projetos embedded o printf não tem
     * suporte a float.
     * Canal 0 = 87.0 MHz, canal 46 = 91.6 MHz, spacing = 100 kHz.
     */
    uint16_t freq10 = 870U + channel;
    uint16_t mhz = freq10 / 10U;
    uint16_t decimal = freq10 % 10U;

    snprintf(out, out_size, "%u.%u MHz V:%02u", mhz, decimal, volume);
}

static int days_in_month(int year, int month) {
    static const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month < 1) month = 1;
    if (month > 12) month = 12;

    if (month == 2) {
        int leap = ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
        return leap ? 29 : 28;
    }

    return days[month - 1];
}

static void clamp_edit_datetime(void) {
    int year = editDateTime.tm_year + 1900;
    int month = editDateTime.tm_mon + 1;
    int max_day;

    if (editDateTime.tm_hour < 0) editDateTime.tm_hour = 23;
    if (editDateTime.tm_hour > 23) editDateTime.tm_hour = 0;

    if (editDateTime.tm_min < 0) editDateTime.tm_min = 59;
    if (editDateTime.tm_min > 59) editDateTime.tm_min = 0;

    if (editDateTime.tm_sec < 0) editDateTime.tm_sec = 59;
    if (editDateTime.tm_sec > 59) editDateTime.tm_sec = 0;

    if (year < 1980) editDateTime.tm_year = 1980 - 1900;
    if (year > 2099) editDateTime.tm_year = 2099 - 1900;

    if (editDateTime.tm_mon < 0) editDateTime.tm_mon = 11;
    if (editDateTime.tm_mon > 11) editDateTime.tm_mon = 0;

    year = editDateTime.tm_year + 1900;
    month = editDateTime.tm_mon + 1;
    max_day = days_in_month(year, month);

    if (editDateTime.tm_mday < 1) editDateTime.tm_mday = max_day;
    if (editDateTime.tm_mday > max_day) editDateTime.tm_mday = 1;
}

static void load_edit_datetime(void) {
    if (CLOCK_GetTimeDate(&editDateTime) != SUCCESS) {
        memset(&editDateTime, 0, sizeof(editDateTime));
        editDateTime.tm_mday = 1;
        editDateTime.tm_year = 2026 - 1900;
    }

    clamp_edit_datetime();
}





static void display_line(uint8_t row, const char *text) {
    char line[17];
    snprintf(line, sizeof(line), "%-16.16s", text);
    DISPLAY_Cursor(0, row);
    DISPLAY_Printf("%s", line);
}

static void render_main(void) {
    tm now;
    char line0[17];
    char line1[17];
    Radio_Data snapshot;

    MENU_Lock();
    snapshot = menuRadioData;
    MENU_Unlock();

    if (CLOCK_GetTimeDate(&now) == SUCCESS) {
        strftime(line0, sizeof(line0), "%H:%M %d/%m/%y", &now);
    } else {
        snprintf(line0, sizeof(line0), "--:-- --/--/----");
    }

    format_frequency(line1, sizeof(line1), snapshot.channel, snapshot.volume);

    display_line(0, line0);
    display_line(1, line1);
}

static void render_options(void) {
    static const char *options[MENU_OPTION_COUNT] = {
        "Clock",
        "Calendar",
        "NTP Sync",
        "Save",
        "Del. Freq.",
        "Del. Volume"
    };

    char line0[17];
    snprintf(line0, sizeof(line0), "> %s", options[currentOption]);

    display_line(0, "Maintenance");
    display_line(1, line0);
}

static void render_confirm(const char *question) {
    char line1[17];
    snprintf(line1, sizeof(line1), "%sYes  %sNo",
             confirmSelection == 0 ? ">" : " ",
             confirmSelection == 1 ? ">" : " ");

    display_line(0, question);
    display_line(1, line1);
}

static void render_set_clock(void) {
    char line0[17];
    char line1[17];

    snprintf(line0, sizeof(line0), "Clock  %s",
             editField == 0 ? "HH" : (editField == 1 ? "MM" : "SS"));
    snprintf(line1, sizeof(line1), "%02d:%02d:%02d",
             editDateTime.tm_hour, editDateTime.tm_min, editDateTime.tm_sec);

    display_line(0, line0);
    display_line(1, line1);
}

static void render_set_date(void) {
    char line0[17];
    char line1[17];

    snprintf(line0, sizeof(line0), "Calendar %s",
             editField == 0 ? "DD" : (editField == 1 ? "MM" : "YYYY"));
    snprintf(line1, sizeof(line1), "%02d/%02d/%04d",
             editDateTime.tm_mday, editDateTime.tm_mon + 1, editDateTime.tm_year + 1900);

    display_line(0, line0);
    display_line(1, line1);
}

static void render_current_screen(void) {
    switch (currentScreen) {
        case MENU_SCREEN_MAIN:
            render_main();
            break;

        case MENU_SCREEN_OPTIONS:
            render_options();
            break;

        case MENU_SCREEN_CONFIRM_DEL_FREQ:
            render_confirm("Del. freq data?");
            break;

        case MENU_SCREEN_CONFIRM_DEL_VOL:
            render_confirm("Del. vol data?");
            break;

        case MENU_SCREEN_SET_CLOCK:
            render_set_clock();
            break;

        case MENU_SCREEN_SET_DATE:
            render_set_date();
            break;

        default:
            currentScreen = MENU_SCREEN_MAIN;
            render_main();
            break;
    }
}

static void apply_volume_delta(int8_t delta) {
    Radio_Data data;
    I2C_MState i2cState;

    MENU_Lock();
    data = menuRadioData;

    if (delta > 0 && data.volume < MENU_MAX_VOLUME) {
        data.volume++;
    } else if (delta < 0 && data.volume > 0) {
        data.volume--;
    }

    menuRadioData = data;
    MENU_Unlock();

    RADIO_RTOS_SET_VOLUME(&data, &i2cState);
}

static void apply_channel_delta(int8_t delta) {
    Radio_Data data;
    I2C_MState i2cState;

    MENU_Lock();
    data = menuRadioData;

    if (delta > 0 && data.channel < MENU_MAX_CHANNEL) {
        data.channel++;
    } else if (delta < 0 && data.channel > MENU_MIN_CHANNEL) {
        data.channel--;
    }

    menuRadioData = data;
    MENU_Unlock();

    RADIO_RTOS_SET_CHANNEL(&data, &i2cState);
}

static void delete_frequency_data(void) {
    Radio_Data data;
    I2C_MState i2cState;

    MENU_Lock();
    data = menuRadioData;
    data.channel = MENU_START_CHANNEL;
    menuRadioData = data;
    MENU_Unlock();

    RADIO_RTOS_SET_CHANNEL(&data, &i2cState);
}

static void delete_volume_data(void) {
    Radio_Data data;
    I2C_MState i2cState;

    MENU_Lock();
    data = menuRadioData;
    data.volume = MENU_START_VOLUME;
    menuRadioData = data;
    MENU_Unlock();

    RADIO_RTOS_SET_VOLUME(&data, &i2cState);
}

static void save_current_data(void) {
    Radio_Data data;
    base_t result;

    MENU_Lock();
    data = menuRadioData;
    MENU_Unlock();

    /*
     * Save the current clock/date together with the current volume/frequency.
     * This is now the ONLY place where the menu explicitly saves to Flash.
     */
    (void)CLOCK_GetTimeDate(&data.time);

    result = RADIO_RTOS_SAVE_DATA(&data);

    if (result == SUCCESS) {
        MENU_Lock();
        menuRadioData = data;
        MENU_Unlock();

        DISPLAY_Clear();
        display_line(0, "Save");
        display_line(1, "OK");
    } else {
        DISPLAY_Clear();
        display_line(0, "Save");
        display_line(1, "FAIL");
    }

    vTaskDelay(pdMS_TO_TICKS(1200));
}

static void start_ntp_sync(void) {
    base_t result;

    result = TIME_SYNC_Start(WIFI_MENU_SSID,
                             WIFI_MENU_PASSWORD,
                             tskIDLE_PRIORITY + 2,
                             configMINIMAL_STACK_SIZE * 8);

    DISPLAY_Clear();

    if (result == SUCCESS) {
        display_line(0, "NTP Sync");
        display_line(1, "Started");
    } else {
        display_line(0, "NTP Sync");
        display_line(1, "FAIL");
    }

    vTaskDelay(pdMS_TO_TICKS(1200));
}

static void handle_main_button(NAVBTN_TypeDef btn) {
    switch (btn) {
        case NAVBTN_UP:
            apply_volume_delta(+1);
            break;

        case NAVBTN_DOWN:
            apply_volume_delta(-1);
            break;

        case NAVBTN_RIGHT:
            apply_channel_delta(+1);
            break;

        case NAVBTN_LEFT:
            apply_channel_delta(-1);
            break;

        case NAVBTN_ENTER:
        case NAVBTN_CENTER:
            currentScreen = MENU_SCREEN_OPTIONS;
            DISPLAY_Clear();
            break;

        default:
            break;
    }
}

static void handle_options_button(NAVBTN_TypeDef btn) {
    switch (btn) {
        case NAVBTN_UP:
            if (currentOption == 0) {
                currentOption = MENU_OPTION_COUNT - 1;
            } else {
                currentOption--;
            }
            break;

        case NAVBTN_DOWN:
            currentOption = (MenuOption)((currentOption + 1) % MENU_OPTION_COUNT);
            break;

        case NAVBTN_BACK:
        case NAVBTN_LEFT:
            currentScreen = MENU_SCREEN_MAIN;
            DISPLAY_Clear();
            break;

        case NAVBTN_ENTER:
        case NAVBTN_CENTER:
            switch (currentOption) {
                case MENU_OPTION_CLOCK:
                    load_edit_datetime();
                    editField = 0;
                    currentScreen = MENU_SCREEN_SET_CLOCK;
                    break;

                case MENU_OPTION_DATE:
                    load_edit_datetime();
                    editField = 0;
                    currentScreen = MENU_SCREEN_SET_DATE;
                    break;

                case MENU_OPTION_NTP_SYNC:
                    start_ntp_sync();
                    currentScreen = MENU_SCREEN_OPTIONS;
                    break;

                case MENU_OPTION_SAVE:
                    save_current_data();
                    currentScreen = MENU_SCREEN_OPTIONS;
                    break;

                case MENU_OPTION_DEL_FREQ:
                    confirmSelection = 0;
                    currentScreen = MENU_SCREEN_CONFIRM_DEL_FREQ;
                    break;

                case MENU_OPTION_DEL_VOL:
                    confirmSelection = 0;
                    currentScreen = MENU_SCREEN_CONFIRM_DEL_VOL;
                    break;

                default:
                    currentScreen = MENU_SCREEN_MAIN;
                    break;
            }
            DISPLAY_Clear();
            break;

        default:
            break;
    }
}

static void handle_confirm_button(NAVBTN_TypeDef btn) {
    switch (btn) {
        case NAVBTN_LEFT:
        case NAVBTN_RIGHT:
        case NAVBTN_UP:
        case NAVBTN_DOWN:
            confirmSelection = !confirmSelection;
            break;

        case NAVBTN_ENTER:
        case NAVBTN_CENTER:
            if (confirmSelection == 0) {
                if (currentScreen == MENU_SCREEN_CONFIRM_DEL_FREQ) {
                    delete_frequency_data();
                } else if (currentScreen == MENU_SCREEN_CONFIRM_DEL_VOL) {
                    delete_volume_data();
                }
            }
            currentScreen = MENU_SCREEN_OPTIONS;
            DISPLAY_Clear();
            break;

        case NAVBTN_BACK:
            currentScreen = MENU_SCREEN_OPTIONS;
            DISPLAY_Clear();
            break;

        default:
            break;
    }
}

static void handle_clock_or_date_button(NAVBTN_TypeDef btn) {
    uint8_t maxField = (currentScreen == MENU_SCREEN_SET_CLOCK) ? 2U : 2U;

    switch (btn) {
        case NAVBTN_LEFT:
            if (editField == 0) {
                editField = maxField;
            } else {
                editField--;
            }
            break;

        case NAVBTN_RIGHT:
            editField = (uint8_t)((editField + 1U) % (maxField + 1U));
            break;

        case NAVBTN_UP:
        case NAVBTN_DOWN: {
            int delta = (btn == NAVBTN_UP) ? 1 : -1;

            if (currentScreen == MENU_SCREEN_SET_CLOCK) {
                if (editField == 0) {
                    editDateTime.tm_hour += delta;
                } else if (editField == 1) {
                    editDateTime.tm_min += delta;
                } else {
                    editDateTime.tm_sec += delta;
                }
            } else {
                if (editField == 0) {
                    editDateTime.tm_mday += delta;
                } else if (editField == 1) {
                    editDateTime.tm_mon += delta;
                } else {
                    editDateTime.tm_year += delta;
                }
            }

            clamp_edit_datetime();
            break;
        }

        case NAVBTN_ENTER:
        case NAVBTN_CENTER:
            clamp_edit_datetime();
            CLOCK_SetTimeDate(&editDateTime);
            currentScreen = MENU_SCREEN_OPTIONS;
            DISPLAY_Clear();
            break;

        case NAVBTN_BACK:
            currentScreen = MENU_SCREEN_OPTIONS;
            DISPLAY_Clear();
            break;

        default:
            break;
    }
}

static void handle_button(NAVBTN_TypeDef btn) {
    if (btn == NAVBTN_NONE) {
        return;
    }

    switch (currentScreen) {
        case MENU_SCREEN_MAIN:
            handle_main_button(btn);
            break;

        case MENU_SCREEN_OPTIONS:
            handle_options_button(btn);
            break;

        case MENU_SCREEN_CONFIRM_DEL_FREQ:
        case MENU_SCREEN_CONFIRM_DEL_VOL:
            handle_confirm_button(btn);
            break;

        case MENU_SCREEN_SET_CLOCK:
        case MENU_SCREEN_SET_DATE:
            handle_clock_or_date_button(btn);
            break;

        default:
            currentScreen = MENU_SCREEN_MAIN;
            break;
    }
}

static void MENU_Task(void *params) {
    (void)params;

    NAVBTN_TypeDef btn;
    TickType_t lastRefresh = 0;

    DISPLAY_Clear();
    render_current_screen();

    while (1) {
        if (BUTTON_Pressed(&btn) == SUCCESS && btn != NAVBTN_NONE) {
            handle_button(btn);
            render_current_screen();
        }

        if ((xTaskGetTickCount() - lastRefresh) >= pdMS_TO_TICKS(MENU_REFRESH_MS)) {
            lastRefresh = xTaskGetTickCount();
            render_current_screen();
        }

        vTaskDelay(pdMS_TO_TICKS(MENU_BUTTON_DELAY_MS));
    }
}

base_t MENU_RTOS_Init(Radio_Data *initial_data) {
    if (initialized) {
        return SUCCESS;
    }

    menuDataMutex = xSemaphoreCreateMutex();
    if (menuDataMutex == NULL) {
        return ERROR;
    }

    memset(&menuRadioData, 0, sizeof(menuRadioData));

    if (initial_data != NULL) {
        menuRadioData = *initial_data;
    } else {
        menuRadioData.channel = MENU_START_CHANNEL;
        menuRadioData.volume = MENU_START_VOLUME;
    }

    initialized = 1;
    return SUCCESS;
}

base_t MENU_RTOS_Start(UBaseType_t priority, uint16_t stack_size) {
    if (!initialized) {
        return ERROR;
    }

    if (menuTaskHandle != NULL) {
        return SUCCESS;
    }

    if (xTaskCreate(MENU_Task, "menu", stack_size, NULL, priority, &menuTaskHandle) != pdPASS) {
        return ERROR;
    }

    return SUCCESS;
}

base_t MENU_RTOS_SetRadioData(const Radio_Data *data) {
    if (data == NULL) {
        return ARG_ERROR;
    }

    MENU_Lock();
    menuRadioData = *data;
    MENU_Unlock();

    return SUCCESS;
}

base_t MENU_RTOS_GetRadioData(Radio_Data *data) {
    if (data == NULL) {
        return ARG_ERROR;
    }

    MENU_Lock();
    *data = menuRadioData;
    MENU_Unlock();

    return SUCCESS;
}
