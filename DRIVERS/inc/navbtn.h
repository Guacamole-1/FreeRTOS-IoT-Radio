/**
 * @file navbtn.h
 * @brief Navigation Buttons Driver API.
 *
 * This module provides an interface to a navigation keypad connected
 * to the LPC1769 microcontroller (e.g., LPCXpresso board).
 * It allows initialization and detection of directional and control keys.
 *
 * @version 1.0
 * @date 27 Feb 2025
 * @author PSampaio
 *
 * @note This software is provided for educational purposes.
 * @warning Provided "AS IS", without any warranties.
 */

#ifndef NAVBTN_H_
#define NAVBTN_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup DRIVERS
 * @{
 */

/**
 * @defgroup NAVBTN NAVBTN — Navigation Buttons Driver
 * @brief Driver for a navigation keypad.
 *
 * Supports directional navigation and control keys, providing a simple
 * and portable interface for user input.
 * @{
 */

/**
 * @brief Navigation keypad button codes.
 */
typedef enum {
    NAVBTN_NONE = -1,   /**< No key pressed */
    NAVBTN_UP = 0,      /**< Up key */
    NAVBTN_DOWN,        /**< Down key */
    NAVBTN_LEFT,        /**< Left key */
    NAVBTN_RIGHT,       /**< Right key */
    NAVBTN_CENTER,      /**< Center / OK key */
    NAVBTN_BACK,        /**< Back key */
    NAVBTN_ENTER,       /**< Enter key */
    NAVBTN_KEY_COUNT    /**< Total number of keys */
} NAVBTN_TypeDef;

/**
 * @brief Initializes the navigation keypad interface.
 *
 * Configures the GPIO pins used by the keypad and prepares the system
 * for key scanning.
 */
void NAVBTN_Init(void);

/**
 * @brief Reads the current key state.
 *
 * Performs a keypad scan and returns the currently pressed key.
 *
 * @retval NAVBTN_NONE No key pressed
 * @retval Other       Key currently pressed
 *
 * @note This function is non-blocking.
 */
NAVBTN_TypeDef NAVBTN_Read(void);

/**
 * @brief Detects a new key press (edge detection).
 *
 * Returns a key code only when a key transitions from released to pressed.
 *
 * @retval NAVBTN_NONE No new key press
 * @retval Other       Newly pressed key
 */
NAVBTN_TypeDef NAVBTN_Pressed(void);

/** @} */  // NAVBTN
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* NAVBTN_H_ */
