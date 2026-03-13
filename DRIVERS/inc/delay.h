/**
 * @file delay.h
 * @brief Delay Driver API for LPC1769.
 *
 * This module provides functions to generate time delays with millisecond
 * and microsecond resolution. It uses the SYSTICK timer for millisecond
 * delays and TIMER0 for microsecond delays.
 *
 * @version 1.1
 * @date 10 Oct 2017
 * @author PSampaio
 *
 * @note This software is provided for educational purposes.
 * @warning Busy-wait delays block CPU execution.
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup DRIVERS
 * @{
 */

/**
 * @defgroup DELAY DELAY — Timing and Delay Driver
 * @brief Driver to generate blocking delays.
 *
 * Provides millisecond and microsecond delay functions.
 * @{
 */

/**
 * @brief Initializes the delay system.
 *
 * Configures the SYSTICK timer for 1 ms resolution delays.
 *
 * @retval 0  Initialization successful
 * @retval -1 Initialization failed
 */
int DELAY_Init(void);

/**
 * @brief Waits for a specified number of milliseconds.
 *
 * @param millis Delay time in milliseconds.
 */
void DELAY_Milliseconds(uint32_t millis);

/**
 * @brief Gets elapsed time in milliseconds.
 *
 * @param start Start time (pass 0 to get current time).
 * @return Elapsed time in milliseconds.
 */
uint32_t DELAY_GetElapsedMillis(uint32_t start);

/**
 * @brief Waits for a specified number of microseconds.
 *
 * @param micros Delay time in microseconds.
 */
void DELAY_Microseconds(uint32_t micros);

/** @} */  // DELAY
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* DELAY_H_ */
