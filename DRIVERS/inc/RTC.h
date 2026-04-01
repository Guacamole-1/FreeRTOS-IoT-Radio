/**
* @file RTC.h
* @brief This header file provides the API for controlling the RTC
* @version 1.1
* @date 4 Nov 2025
* @author Grupo06
*
*/

/**
* @addtogroup DRIVERS
* @{
*/

/**
* @defgroup RTC RTC functions
* @brief This module provides core functions for the Real Time Clock
* @{
*/

#ifndef RTC_H_
#define RTC_H_

#include "LPC17xx.h"
#include <stdbool.h>
#include <time.h>

#define SC LPC_SC 
#define PC_RTC 9
#define RTC LPC_RTC


#define BIT_MASK(width)           ((1U << (width)) - 1U)
#define GET_BITS(value, start, end) (((value) >> (start)) & BIT_MASK(end-start+1))
// start starts from 0

/**
 * @brief limits value to a range between min and max
 * */
int clamp(int value,int min,int max);

typedef struct tm tm;

/**
 * @brief initializes the RTC, it's initialized with the time_t seconds
 * @param seconds passsed since 00:00:00 UTC of 1 january 1970.
 * @warning must be called before other RTC functions for them to work properly
 * */
void RTC_Init(time_t seconds);

/**
 * @brief fills the tm struct with values
 * @param dateTime tm struct to be filled
 * @note it gets the values for the dateTime struct to avoid passing wrong information
 * the day changes between the two times it gets the date for comparison
 * */
void RTC_GetTimeDate(tm *dateTime);

/**
 * @brief Updates RTC with values from dateTime struct to be set
 * @param dateTime struct which contains the date
 * */
void RTC_SetTimeDate(tm *dateTime);

/**
 * @brief Updates RTC with value seconds
 * @param seconds represents amount of seconds passed
 * since 00:00:00 UTC of 1 january 1970.
 * */
void RTC_SetSeconds(time_t seconds);

/**
 * @brief returns seconds passed since 00:00:00 UTC of 1 january 1970.
 * @returns time_t
 * */
time_t RTC_GetSeconds(void);

#endif /* RTC_H_ */

/**
* @}
*/

/**
* @}
*/
