/*
 * clock.c
 *
 *  Created on: 25/03/2026
 *      Author: rafav
 */

#include "FreeRTOS.h"

/* Kernel includes. */
#include "portable.h"
#include "projdefs.h"
#include "task.h"
#include "queue.h"

#include "clock.h"
#include "RTC.h"

static SemaphoreHandle_t xSemaphore;



CLOCK_STATUS CLOCK_Init(){
	if(!RTC_Init()){
		return CLOCK_INIT_ERROR;
	}

	xSemaphore = xSemaphoreCreateMutex();
	xSemaphoreGive(xSemaphore);


	return CLOCK_SUCCESS;
}

CLOCK_STATUS CLOCK_SetTimeDate(tm *dateTime){

	xSemaphoreTake(xSemaphore,portMAX_DELAY);
	RTC_SetTimeDate(tm *dateTime);
	xSemaphoreGive(xSemaphore);
}

CLOCK_STATUS CLOCK_GetTimeDate(tm *dateTime){

	xSemaphoreTake(xSemaphore,portMAX_DELAY);
	RTC_GetTimeDate(tm *dateTime);
	xSemaphoreGive(xSemaphore);
}

CLOCK_STATUS CLOCK_GetSeconds(time_t seconds){

	xSemaphoreTake(xSemaphore,portMAX_DELAY);
	RTC_GetSeconds(tm *dateTime);
	xSemaphoreGive(xSemaphore);
}

CLOCK_STATUS CLOCK_SetSeconds(time_t seconds){

	xSemaphoreTake(xSemaphore,portMAX_DELAY);
	RTC_SetSeconds(tm *dateTime);
	xSemaphoreGive(xSemaphore);
}



