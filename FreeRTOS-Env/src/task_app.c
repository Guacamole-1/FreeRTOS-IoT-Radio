#include <menu_rtos.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "FreeRTOS.h"
#include "Wifi.h"
#include "task.h"

#include "base.h"
#include "display.h"
#include "button.h"
#include "clock.h"
#include "radio_rtos.h"
#include "publisher.h"
#include "wifi_rtos.h"
#include "time_sync.h"
#include "Menu.h"

void APP_Task(void *pvParameters) {
  (void)pvParameters;
  time_t time;
  printf("1");
  if (DISPLAY_Init() != SUCCESS) {
    goto exit_task;
  }
  printf("2");

  if (BUTTON_Init() != SUCCESS) {
    goto exit_task;
  }
  printf("3");
  if (WIFI_RTOS_Init()) {
    goto exit_task;
  }
  printf("4");
  if (WIFI_RTOS_SetModeStation()) {
    goto exit_task;
  }
  printf("4.1");
  if (WIFI_RTOS_ConnectAp(SSID, PASS)) {
    goto exit_task;
  }
  printf("4.2");
  if (!TIME_SYNC_RequestUnixTime(&time)) {
    goto exit_task;
  }
  printf("5");
  if (CLOCK_Init(time) != SUCCESS) {
    goto exit_task;
  }
  Radio_Data radio_data = {.id = RADIO_DATA_ID,
                           .channel = 46, /* 91.6 MHz Cidade FM */
                           .volume = 8,
                           .time = {0}};
  printf("5");

  CLOCK_GetTimeDate(&radio_data.time);

  if (RADIO_RTOS_INIT(&radio_data) != SUCCESS) {
    goto exit_task;
  }

  /* printf("7"); */
  /* if (MENU_RTOS_Init(&radio_data) != SUCCESS) { */
  /*   goto exit_task; */
  /* } */
  /* printf("8"); */
  /* if (MENU_RTOS_Start(tskIDLE_PRIORITY + 1, configMINIMAL_STACK_SIZE * 3) != */
  /*     SUCCESS) { */
  /*   goto exit_task; */
  /* } */
  printf("9");

  if (Start_Publisher_Task(&radio_data) != SUCCESS) {
    goto exit_task;
  }
  printf("10");
  Menu_Init(&radio_data);
  if (xTaskCreate(Main_Menu, "Main Menu", configMINIMAL_STACK_SIZE * 5, NULL,
                  tskIDLE_PRIORITY +1, NULL) != pdPASS) {
      goto exit_task;
  }

  DISPLAY_Manager();

exit_task:
  printf("task deleted");
  vTaskDelete(NULL);
}
// TODO: mudar o menu_rtos.c
 
int32_t App(void) {

  if (xTaskCreate(APP_Task, "App", configMINIMAL_STACK_SIZE * 6, NULL, // change to 3
                  tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
    return -1;
  }

  return 0;
}
