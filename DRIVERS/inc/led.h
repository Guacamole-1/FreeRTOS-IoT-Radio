/**
 * @file led.h
 * @brief LED Driver API for LPC1769 (LPCXpresso board).
 *
 * This module provides a simple interface to control the on-board LED.
 * It supports initialization and basic LED state operations.
 *
 * @version 1.0
 * @date 9 Oct 2018
 * @author PSampaio
 *
 * @note This software is provided for educational purposes.
 * @warning Provided "AS IS", without any warranties.
 */

#ifndef LED_H_
#define LED_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup DRIVERS
 * @{
 */

/**
 * @defgroup LED LED — Board LED Driver
 * @brief Driver to control the on-board LED.
 *
 * Provides basic LED control functions: initialize, turn on, turn off and toggle.
 * @{
 */

/**
 * @brief Initializes the LED control interface.
 *
 * Configures the GPIO pin connected to the LED and sets its initial state.
 *
 * @param state Initial LED state:
 *              - false → LED off
 *              - true  → LED on
 */
void LED_Init(bool state);

/**
 * @brief Gets the current LED state.
 *
 * @retval true  LED is on
 * @retval false LED is off
 */
bool LED_GetState(void);

/** @brief Turns the LED on. */
void LED_On(void);

/** @brief Turns the LED off. */
void LED_Off(void);

/** @brief Toggles the LED state. */
void LED_Toggle(void);

/** @} */  // LED
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* LED_H_ */
