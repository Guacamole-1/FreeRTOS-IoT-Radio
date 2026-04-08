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

base_t (* RTC_GetSeconds )(time_t* seconds) = CLOCK_GetSeconds;
base_t (* RTC_GetTimeDate )(tm *dateTime) = CLOCK_GetTimeDate;
base_t (* RTC_SetTimeDate )(tm *dateTime) = CLOCK_SetTimeDate;
base_t (* RTC_Init )(time_t seconds) = CLOCK_Init;
base_t (* RTC_SetSeconds )(time_t seconds) = CLOCK_SetSeconds;



base_t CLOCK_Init(time_t seconds){

	RTC_Init(seconds);
	LOCK_INIT;
	return SUCCESS;
}

base_t CLOCK_SetTimeDate(tm *dateTime){

	if(dateTime == NULL) return ARG_ERROR;

	LOCK
		RTC_SetTimeDate(dateTime);
	UNLOCK
	return SUCCESS;
}

base_t CLOCK_GetTimeDate(tm *dateTime){

  if (dateTime == NULL) return ARG_ERROR;

  LOCK
	RTC_GetTimeDate(dateTime);
  UNLOCK

  return SUCCESS;
}

base_t CLOCK_GetSeconds(time_t* seconds){

  if (seconds == NULL) return ARG_ERROR;

  LOCK
	*seconds = RTC_GetSeconds();
  UNLOCK

  return SUCCESS;
}

base_t CLOCK_SetSeconds(time_t seconds){

	LOCK
		RTC_SetSeconds(seconds);
	UNLOCK

	return SUCCESS;
}

