/*
 * button.c
 *
 *  Created on: 01/04/2026
 *      Author: rafav
 */


#include "button.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"

#define BTN_SCAN_PERIOD_MS      200
#define BTN_QUEUE_LENGTH        8

static QueueHandle_t btnQueue = NULL;
static TimerHandle_t scanTimer = NULL;





static void vScanTimerCallback(TimerHandle_t xTimer) {
    BUTTON_TypeDef pressedButton = NAVBTN_Pressed();

        if (pressedButton != NAVBTN_NONE) {
        	xQueueSend(btnQueue, &pressedButton, BUTTON_TICKS_TO_WAIT);
        }

        pressedButton = NAVBTN_NONE;
}


BUTTON_STATUS BUTTON_RTOS_Init(void) {
    NAVBTN_Init();

    btnQueue = xQueueCreate(BTN_QUEUE_LENGTH, sizeof(BUTTON_TypeDef));
    if (btnQueue == NULL) {
        return BUTTON_INIT_FAIL;
    }

    scanTimer = xTimerCreate(
        "BtnScan",
        pdMS_TO_TICKS(BTN_SCAN_PERIOD_MS),
        pdTRUE,         /* auto-reload */
        NULL,
        vScanTimerCallback
    );

    if (scanTimer == NULL) {
        return BUTTON_INIT_FAIL;
    }


    if (xTimerStart(scanTimer, 0) != pdPASS) {
        return BUTTON_INIT_FAIL;
    }

    return BUTTON_SUCCESS;
}

