#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <task_app.h>
#include <time.h>

/* Kernel includes. */
#include "FreeRTOS.h"

/* Kernel includes. */
#include "LCD.h"
#include "projdefs.h"
#include "task.h"
#include "queue.h"

#include "task_app.h"
#include "semphr.h"

// DRIVER includes
#include "display.h"

void Messages_Task(void *pvParameters) {
  vTaskDelay(pdMS_TO_TICKS(3000));
  DISPLAY_Args_WS arg = {"hello\n"};
  DISPLAY_Item msg = {WRITE_STR, &arg};
  DISPLAY_Send(msg);
  lcd16_t curs = CURSOR_SETUP(CURSOR_ON|BLINK_ON);
  DISPLAY_Send((DISPLAY_Item){WRITE_CMD, &curs});
  vTaskDelay(pdMS_TO_TICKS(3000));
  curs = CURSOR_SETUP(CURSOR_ON | BLINK_OFF);
  DISPLAY_Send((DISPLAY_Item){WRITE_CMD, &curs});
  vTaskDelay(pdMS_TO_TICKS(3000));
  curs = CURSOR_SETUP(CURSOR_OFF | BLINK_ON);
  DISPLAY_Send((DISPLAY_Item){WRITE_CMD, &curs});
  vTaskDelay(pdMS_TO_TICKS(3000));
  curs = CURSOR_SETUP(CURSOR_OFF | BLINK_OFF);
  DISPLAY_Send((DISPLAY_Item){WRITE_CMD, &curs});
  char* str = "world";
  vTaskDelay(pdMS_TO_TICKS(1000));
  DISPLAY_Printf("%s :)",str);
  vTaskDelay(pdMS_TO_TICKS(1000));
  DISPLAY_Send((DISPLAY_Item){CLEAR});
  vTaskDelay(pdMS_TO_TICKS(1000));
  Cursor c = {10,1};
  DISPLAY_Send((DISPLAY_Item){CURSOR_SET,&c});
  DISPLAY_Args_WS arg2 = {":P"};
  DISPLAY_Item msg2 = {WRITE_STR,&arg2};
  DISPLAY_Send(msg2);
  vTaskDelete(NULL);
}


void APP_Task(void *pvParameters) {
	if(DISPLAY_Init() != DISP_SUCCESS){
	    	return;
	    }
	DISPLAY_Manager();
}


int32_t App(void) {

    if (xTaskCreate(APP_Task, "App", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        return -1;
    }
    if (xTaskCreate(Messages_Task, "msg_sender", configMINIMAL_STACK_SIZE*2, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
      return -1;
    }
    return 0;
}

