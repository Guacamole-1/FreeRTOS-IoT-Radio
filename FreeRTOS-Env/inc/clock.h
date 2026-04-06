/**
 * @file clock.h
 * @brief This header file provides the API for controlling the RTC with
 * concurrent access
 * @version 1
 * @date 25 Mar 2026
 * @author Grupo 08
 *
 */

/**
 * @defgroup clock_module Clock Module
 * @ingroup free_rtos
 * @brief Asynchronous RTC manager.
 * @{
 */
#ifndef CLOCK_H_

#include "RTC.h"

typedef enum {
	CLOCK_LOCK_ERROR = -1,
	CLOCK_SUCCESS,
	CLOCK_ERROR
} CLOCK_STATUS;

/**
 * @brief Initializes the clock module and the underlying RTC.
 *
 * This function must be called before any other CLOCK_ function.
 * It initializes the RTC with the number of seconds elapsed since
 * 00:00:00 UTC, 1 January 1970, and also initializes the internal lock
 * used to protect concurrent access.
 *
 * @param seconds Number of seconds since 00:00:00 UTC, 1 January 1970.
 *
 * @return CLOCK_SUCCESS on success, CLOCK_LOCK_ERROR if the internal lock
 *         could not be created.
 */
CLOCK_STATUS CLOCK_Init(time_t seconds);

/**
 * @brief Sets the current date and time in the RTC.
 *
 * Updates the RTC using the values stored in the provided `tm` structure.
 *
 * @param dateTime Pointer to a `tm` structure containing the date and time
 *                 to be written to the RTC.
 *
 * @return CLOCK_SUCCESS on success, CLOCK_LOCK_ERROR if the module lock
 *         could not be acquired.
 */
CLOCK_STATUS CLOCK_SetTimeDate(tm *dateTime);

 /**
 * @brief Reads the current date and time from the RTC.
 *
 * Fills the provided `tm` structure with the current RTC date and time.
 * This operation is protected so that the date and time are read consistently.
 *
 * @param dateTime Pointer to a `tm` structure to be filled with the current
 *                 date and time.
 *
 * @return CLOCK_SUCCESS on success, CLOCK_LOCK_ERROR if the module lock
 *         could not be acquired.
 */
CLOCK_STATUS CLOCK_GetTimeDate(tm *dateTime);

/**
 * @brief Reads the current RTC time as Unix time.
 *
 * Retrieves the number of seconds elapsed since 00:00:00 UTC,
 * 1 January 1970.
 *
 * @param seconds Pointer to a `time_t` variable where the result will be stored.
 *
 * @return CLOCK_SUCCESS on success, CLOCK_LOCK_ERROR if the module lock
 *         could not be acquired.
 */
CLOCK_STATUS CLOCK_GetSeconds(time_t *seconds);

/**
 * @brief Sets the RTC using Unix time.
 *
 * Updates the RTC with the number of seconds elapsed since
 * 00:00:00 UTC, 1 January 1970.
 *
 * @param seconds Number of seconds since 00:00:00 UTC, 1 January 1970.
 *
 * @return CLOCK_SUCCESS on success, CLOCK_LOCK_ERROR if the module lock
 *         could not be acquired.
 */
CLOCK_STATUS CLOCK_SetSeconds(time_t seconds);

#define CLOCK_H_

#endif /* CLOCK_H_ */

/** @} */
