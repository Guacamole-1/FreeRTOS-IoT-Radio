/**
 * @file pinconfig.h
 * @brief Pin Configuration Driver API for LPC1769.
 *
 * This module provides a high-level interface to configure the pin
 * multiplexing, pull resistors and open-drain mode of LPC1769 pins.
 *
 * @version 1.1
 * @date 18 Nov 2018
 * @author PSampaio
 *
 * @note This software is provided for educational purposes.
 * @warning Provided "AS IS", without any warranties.
 */

#ifndef PINCONFIG_H_
#define PINCONFIG_H_

#include "pin.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup DRIVERS
 * @{
 */

/**
 * @defgroup PINCONFIG PINCONFIG — Pin Configuration Driver
 * @brief Driver to configure LPC1769 pin electrical and functional behavior.
 *
 * This module allows selecting the pin function (GPIO, UART, SPI, I2C, etc.),
 * enabling pull-up or pull-down resistors and configuring open-drain mode.
 * @{
 */

/**
 * @brief Pin function selection.
 */
typedef enum {
    PINCFG_FUNC_GPIO      = 0,  /**< GPIO function */
    PINCFG_FUNC_UART0     = 1,  /**< UART0 function */
    PINCFG_FUNC_UART1     = 1,  /**< UART1 function */
    PINCFG_FUNC_UART1_ALT = 2,  /**< Alternate UART1 function */
    PINCFG_FUNC_UART2     = 1,  /**< UART2 function */
    PINCFG_FUNC_UART3     = 2,  /**< UART3 function */
    PINCFG_FUNC_UART3_ALT = 3,  /**< Alternate UART3 function */
    PINCFG_FUNC_SPI       = 3,  /**< SPI function */
    PINCFG_FUNC_SSP0      = 2,  /**< SSP0 function */
    PINCFG_FUNC_I2C0      = 1,  /**< I2C0 function */
    PINCFG_FUNC_I2C1      = 3,  /**< I2C1 function */
    PINCFG_FUNC_I2C1_ALT  = 3,  /**< Alternate I2C1 function */
    PINCFG_FUNC_I2C2      = 2,  /**< I2C2 function */
    PINCFG_FUNC_PWM1      = 1,  /**< PWM function */
    PINCFG_FUNC_ANALOG    = 1   /**< Analog input function */
} PINCFG_FunctionType;


/**
 * @brief Pin resistor configuration.
 */
typedef enum {
    PINCFG_PULL_UP,     /**< Pull-up resistor enabled */
    PINCFG_REPEATER,    /**< Repeater mode */
    PINCFG_NO_PULL,     /**< No pull-up or pull-down */
    PINCFG_PULL_DOWN    /**< Pull-down resistor enabled */
} PINCFG_ResistorModeType;

/**
 * @brief Pin open-drain configuration.
 */
typedef enum {
    PINCFG_NORMAL,       /**< Normal (push-pull) output */
    PINCFG_OPEN_DRAIN    /**< Open-drain output */
} PINCFG_OpenDrainModeType;

/**
 * @brief Configures the function of a pin.
 *
 * @param pinId Pin identifier.
 * @param func  Function to assign.
 */
void PINCFG_Function(PIN_ValueType pinId, PINCFG_FunctionType func);

/**
 * @brief Configures the pull resistor mode of a pin.
 *
 * @param pinId Pin identifier.
 * @param mode  Resistor configuration.
 */
void PINCFG_ResistorMode(PIN_ValueType pinId, PINCFG_ResistorModeType mode);

/**
 * @brief Configures the open-drain mode of a pin.
 *
 * @param pinId Pin identifier.
 * @param mode  Open-drain configuration.
 */
void PINCFG_OpenDrainMode(PIN_ValueType pinId, PINCFG_OpenDrainModeType mode);

/** @} */  // PINCONFIG
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* PINCONFIG_H_ */
