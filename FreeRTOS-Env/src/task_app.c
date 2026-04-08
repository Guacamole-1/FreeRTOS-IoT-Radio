#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <task_app.h>
#include <time.h>

/* Kernel includes. */
#include "FreeRTOS.h"

/* Kernel includes. */
#include "LCD.h"
#include "Nav7Btn.h"
#include "base.h"
#include "clock.h"
#include "projdefs.h"
#include "task.h"
#include "queue.h"

#include "task_app.h"
#include "semphr.h"

// kernel modules includes
#include "display.h"
#include "button.h"

void Messages_Task(void *pvParameters) {
	vTaskDelay(pdMS_TO_TICKS(3000));
	tm date;
	char str[MAX_CHARS+1];
	while (1) {
		DISPLAY_Send((DISPLAY_Item){CLEAR});
		CLOCK_GetTimeDate(&date);
		strftime(str, sizeof(str),"%H:%M:%S", &date);
		DISPLAY_Send((DISPLAY_Item){WRITE_STR,str});
		vTaskDelay(pdMS_TO_TICKS(100));
	}
	vTaskDelete(NULL);
}


void APP_Task(void *pvParameters) {
	if(DISPLAY_Init() != SUCCESS){
	    	return;
	    }
	else if (BUTTON_Init() != SUCCESS){
		return;
	}
	else if (CLOCK_Init(1775596492) != SUCCESS){
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

