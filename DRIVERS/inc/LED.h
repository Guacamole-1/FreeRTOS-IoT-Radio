/**
* @file LED.h
* @brief This header file provides the API for controlling the LED
* @version 1.1
* @date 07 Out 2025
* @author Grupo06
*
*/


/**
* @addtogroup DRIVERS
* @{
*/

/**
* @defgroup LED LED functions
* @brief This module provides core functions for turning on
* and off the LED.
* @{
*/

#ifndef LED_H_
#define LED_H_

//#ifdef __USE_CMSIS
#include "LPC17xx.h"
//#endif

#include <stdio.h>
#include <stdbool.h>



/**
*  @brief Initializes the system to allow the control of the LED
*  @param state Initial LED state
*  @note This function MUST be called before any LED control function
*/
void LED_Init(bool state);

/** @brief Checks if the LED is ON or OFF
 *  @return True if LED is ON, False if it is OFF
 */
bool LED_GetState(void);

/** @brief Turns on the LED
 * @return void
*/
void LED_On(void);

/** @brief Turns OFF the LED
 * @return void
*/
void LED_Off(void);

/** @brief Switches the LED state, if its ON it will turn OFF and vice versa
 * @return void
*/
void LED_Toggle(void);

#endif /* LED_H_ */

/**
* @}
*/

/**
* @}
*/



