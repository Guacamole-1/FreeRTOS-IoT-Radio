#include <menu_rtos.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "FreeRTOS.h"
#include "Wifi.h"
#include "projdefs.h"
#include "task.h"

#include "base.h"
#include "display.h"
#include "button.h"
#include "clock.h"
#include "radio_rtos.h"
#include "publisher.h"
#include "wifi_rtos.h"
#include "time_sync.h"
#include "menu_rtos.h"

#define TEXT_DELAY 1000

#define TRY_START(func,initial_str)									\
			DISPLAY_Clear();										\
			DISPLAY_Write(initial_str);								\
			err = func;												\
			vTaskDelay(pdMS_TO_TICKS(TEXT_DELAY));							\
			if (err != SUCCESS) {									\
				DISPLAY_Clear();									\
				DISPLAY_Printf("ERROR CODE: %d\ncontinuing..",err); \
				vTaskDelay(pdMS_TO_TICKS(1000));}					\
			else {													\
				DISPLAY_Clear();									\
				DISPLAY_Write("SUCESS");							\
				vTaskDelay(pdMS_TO_TICKS(TEXT_DELAY));}

void APP_Task(void *pvParameters) {
	(void)pvParameters;
	time_t time;
	base_t err;
	DISPLAY_Init();
	TRY_START(DISPLAY_TaskStart(), "Initializing\ndisplay..")

	TRY_START(BUTTON_Init(), "Initializing\nButtons..")

	TRY_START(WIFI_RTOS_Init(),"Initializing\nWIFI..")

	TRY_START(WIFI_RTOS_SetModeStation(), "Putting WIFI\nin station mode..")

	TRY_START(WIFI_RTOS_ConnectAp(SSID, PASS), "Connecting to\nSSID: " SSID)

	TRY_START(!TIME_SYNC_RequestUnixTime(&time), "fetching time\nthrough NTP..")

	TRY_START(CLOCK_Init(time), "Setting Clock..")

	Radio_Data radio_data = {.id = RADIO_DATA_ID,
							 .channel = 46, /* 91.6 MHz Cidade FM */
							 .volume = 8,
							 .time = {0}};

	CLOCK_GetTimeDate(&radio_data.time);

	TRY_START(RADIO_RTOS_INIT(&radio_data), "Starting Radio..")

	TRY_START(Start_Publisher_Task(&radio_data),"Starting MQTT..")

	Menu_Init(&radio_data);

	TRY_START(SUCCESS,"Initializing\nMenu..")

	Main_Menu();

	vTaskDelete(NULL);
}

int32_t App(void) {

	if (xTaskCreate(APP_Task, "App", configMINIMAL_STACK_SIZE * 6, NULL, // change to 3
					tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		return -1;
	}

	return 0;
}
