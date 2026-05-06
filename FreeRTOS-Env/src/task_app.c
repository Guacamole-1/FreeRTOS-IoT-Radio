#include <menu_rtos.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"

#include "base.h"
#include "display.h"
#include "button.h"
#include "clock.h"
#include "radio_rtos.h"

void APP_Task(void *pvParameters) {
    (void)pvParameters;

    if (DISPLAY_Init() != SUCCESS) {
        vTaskDelete(NULL);
    }

    if (BUTTON_Init() != SUCCESS) {
        vTaskDelete(NULL);
    }

    if (CLOCK_Init(1775596492) != SUCCESS) {
        vTaskDelete(NULL);
    }

    Radio_Data radio_data = {
        .id = RADIO_DATA_ID,
        .channel = 46,   /* 91.6 MHz Cidade FM */
        .volume = 8,
        .time = {0}
    };

    /*
     * RADIO_RTOS_INIT tries to read saved data from Flash.
     * If valid saved data exists, it replaces radio_data and configures the radio.
     */
    if (RADIO_RTOS_INIT(&radio_data) != SUCCESS) {
        vTaskDelete(NULL);
    }

    /*
     * Only apply saved time/date if it looks valid.
     */
    if (radio_data.time.tm_year >= (1980 - 1900) &&
        radio_data.time.tm_year <= (2099 - 1900) &&
        radio_data.time.tm_mon >= 0 &&
        radio_data.time.tm_mon <= 11 &&
        radio_data.time.tm_mday >= 1 &&
        radio_data.time.tm_mday <= 31) {

        (void)CLOCK_SetTimeDate(&radio_data.time);
    }

    if (MENU_RTOS_Init(&radio_data) != SUCCESS) {
        vTaskDelete(NULL);
    }

    if (MENU_RTOS_Start(tskIDLE_PRIORITY + 1, configMINIMAL_STACK_SIZE * 3) != SUCCESS) {
        vTaskDelete(NULL);
    }

    /*
     * No automatic NTP sync here.
     * NTP is now triggered from the menu option "NTP Sync".
     * Flash save is now triggered from the menu option "Save".
     */
    DISPLAY_Manager();

    vTaskDelete(NULL);
}

int32_t App(void) {

    if (xTaskCreate(APP_Task,
                    "App",
                    configMINIMAL_STACK_SIZE * 3,
                    NULL,
                    tskIDLE_PRIORITY + 1,
                    NULL) != pdPASS) {
        return -1;
    }

    return 0;
}
