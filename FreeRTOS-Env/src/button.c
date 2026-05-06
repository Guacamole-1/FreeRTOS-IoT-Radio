/**
 * @file display.c
 * @brief This source file implements the API for controlling the NAV7BTN with concurrent access
 * @version 1
 * @date 1 Apr 2026
 * @author Grupo 08
 *
 */

#include "button.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "Nav7Btn.h"
#include "base.h"
#include "portmacro.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "mutex_wrapper.h"
#define BTN_SCAN_PERIOD_MS      100
#define BTN_QUEUE_LENGTH        8

static QueueHandle_t btnQueue = NULL;
static TimerHandle_t scanTimer = NULL;

LOCK_DEF

static void BUTTON_callback(TimerHandle_t xTimer) {
	if (!INIT_FLAG ) return;
    NAVBTN_TypeDef pressedButton = NAVBTN_Pressed();

	if (pressedButton != NAVBTN_NONE) {
	  xQueueSend(btnQueue, &pressedButton, BUTTON_TICKS_TO_WAIT);
	}
}

base_t BUTTON_Pressed(NAVBTN_TypeDef* pressed){

	configASSERT(pressed != NULL);
	if (pressed == NULL) {return ARG_ERROR;}

	INIT_CHECK();

	if(xQueueReceive(btnQueue,pressed, 0) != pdPASS){
		*pressed = NAVBTN_NONE;
	}

	return SUCCESS;
}



base_t BUTTON_Init(void) {
	INIT_NCHECK();

    NAVBTN_Init();

    btnQueue = xQueueCreate(BTN_QUEUE_LENGTH, sizeof(NAVBTN_TypeDef));
	configASSERT(btnQueue != NULL);

    scanTimer = xTimerCreate(
							"BtnScan",
							pdMS_TO_TICKS(BTN_SCAN_PERIOD_MS),
							pdTRUE,         /* auto-reload */
							NULL,
							BUTTON_callback
							);

	configASSERT(scanTimer != NULL);

	LOCK_INIT();

    if (xTimerStart(scanTimer, portMAX_DELAY) != pdPASS) {
        configASSERT(ERROR);
    }

    return SUCCESS;
}

