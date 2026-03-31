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
#include "semphr.h"
#include "clock.h"
#include "RTC.h"
#include "mutex_wrapper.h"


LOCK_DEF;


CLOCK_STATUS CLOCK_Init(time_t seconds){

	RTC_Init(seconds);
	LOCK_INIT;
	return CLOCK_SUCCESS;
}

CLOCK_STATUS CLOCK_SetTimeDate(tm *dateTime){

	LOCK
		RTC_SetTimeDate(dateTime);
	UNLOCK
	return CLOCK_SUCCESS;
}

CLOCK_STATUS CLOCK_GetTimeDate(tm *dateTime){

	LOCK
		RTC_GetTimeDate(dateTime);
	UNLOCK
	return CLOCK_SUCCESS;
}

CLOCK_STATUS CLOCK_GetSeconds(time_t* seconds){

	LOCK
		*seconds = RTC_GetSeconds();
	UNLOCK

    return CLOCK_SUCCESS;
}

CLOCK_STATUS CLOCK_SetSeconds(time_t seconds){

	LOCK
		RTC_SetSeconds(seconds);
	UNLOCK

	return CLOCK_SUCCESS;
}

