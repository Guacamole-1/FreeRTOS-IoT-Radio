/**
 * @file clock.c
 * @brief This source file implements the API for controlling the RTC with concurrent access
 * @version 1
 * @date 25 Mar 2026
 * @author Grupo 08
 *
 */
#include "FreeRTOS.h"

/* Kernel includes. */
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

	if(dateTime == NULL) return CLOCK_ERROR;

	LOCK
		RTC_SetTimeDate(dateTime);
	UNLOCK
	return CLOCK_SUCCESS;
}

CLOCK_STATUS CLOCK_GetTimeDate(tm *dateTime){

  if (dateTime == NULL) return CLOCK_ERROR;

  LOCK
	RTC_GetTimeDate(dateTime);
  UNLOCK

  return CLOCK_SUCCESS;
}

CLOCK_STATUS CLOCK_GetSeconds(time_t* seconds){

  if (seconds == NULL) return CLOCK_ERROR;

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

