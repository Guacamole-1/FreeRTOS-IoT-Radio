/**
 * @file rtc.h
 * @brief Real-Time Clock (RTC) Driver API for LPC1769.
 *
 * This module provides a high-level interface to configure and use the
 * RTC peripheral. It supports reading and writing date/time in calendar
 * format (`struct tm`) and UNIX timestamp format (`time_t`).
 *
 * @version 1.0
 * @date 30 Oct 2018
 * @author PSampaio
 *
 * @note This software is provided for educational purposes.
 * @warning Provided "AS IS", without any warranties.
 */

#ifndef RTC_H_
#define RTC_H_

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup DRIVERS
 * @{
 */

/**
 * @defgroup RTC RTC — Real-Time Clock Driver
 * @brief Driver for the LPC1769 Real-Time Clock peripheral.
 *
 * This module abstracts the hardware registers and provides a simple API
 * to manage date and time.
 * @{
 */

/**
 * @brief Initializes the RTC with a calendar date and time.
 *
 * Loads the provided date/time into the RTC hardware.
 *
 * @param dateTime Pointer to a `struct tm` structure containing the
 *        initial date and time.
 *
 * @note Should be called once at system startup.
 */
void RTC_Init(const struct tm *dateTime);

/**
 * @brief Initializes the RTC using a UNIX timestamp.
 *
 * Converts the UNIX timestamp into calendar format and loads it
 * into the RTC hardware.
 *
 * @param time UNIX time in seconds since 1 January 1970.
 */
void RTC_InitSeconds(time_t time);

/**
 * @brief Reads the current RTC date and time.
 *
 * @param dateTime Pointer to a `struct tm` structure where the current
 *        date and time will be stored.
 */
void RTC_GetValue(struct tm *dateTime);

/**
 * @brief Reads the current RTC time as a UNIX timestamp.
 *
 * @return Current RTC time in seconds since 1 January 1970.
 */
time_t RTC_GetSeconds(void);

/**
 * @brief Sets the RTC date and time using calendar format.
 *
 * @param dateTime Pointer to a `struct tm` structure containing the
 *        new date and time.
 */
void RTC_SetValue(const struct tm *dateTime);

/**
 * @brief Sets the RTC time using a UNIX timestamp.
 *
 * @param time UNIX time in seconds since 1 January 1970.
 */
void RTC_SetSeconds(time_t time);

/** @} */  // RTC
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* RTC_H_ */
