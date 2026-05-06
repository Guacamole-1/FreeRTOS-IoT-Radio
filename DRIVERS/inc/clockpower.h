/**
 * @file clockpower.h
 * @brief Clock and Power Control Driver API for LPC1769.
 *
 * This module provides functions to manage peripheral clocks and power domains.
 * It allows enabling/disabling peripheral clocks and configuring clock divisors.
 *
 * @version 1.0
 * @date 6 Jan 2012
 * @author PSampaio
 *
 * @note This software is provided for educational purposes.
 * @warning Incorrect clock configuration may affect peripheral operation.
 */

#ifndef CLOCKPOWER_H_
#define CLOCKPOWER_H_

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
 * @defgroup CLOCKPOWER CLOCKPOWER — Clock and Power Driver
 * @brief Driver for peripheral clock and power control.
 * @{
 */

/**
 * @brief Peripheral clock selection positions.
 */
typedef enum {
    CLKPWR_TIMER0 = 2,
    CLKPWR_TIMER1 = 4,
    CLKPWR_UART0  = 6,
    CLKPWR_UART1  = 8,
    CLKPWR_PWM1   = 12,
    CLKPWR_I2C0   = 14,
    CLKPWR_SPI    = 16,
    CLKPWR_I2C1   = 38,
    CLKPWR_TIMER2 = 44,
    CLKPWR_UART2  = 48,
    CLKPWR_UART3  = 50,
    CLKPWR_I2C2   = 52,
    CLKPWR_RIT    = 58
} CLKPWR_PeripheralClock;

/**
 * @brief Peripheral clock division factors.
 */
typedef enum {
    CLKPWR_DIV_4 = 0,  /**< Peripheral clock = CCLK / 4 */
    CLKPWR_DIV_1 = 1,  /**< Peripheral clock = CCLK */
    CLKPWR_DIV_2 = 2,  /**< Peripheral clock = CCLK / 2 */
    CLKPWR_DIV_8 = 3   /**< Peripheral clock = CCLK / 8 */
} CLKPWR_ClockDiv;

/**
 * @brief Peripheral power control bits.
 */
typedef enum {
    CLKPWR_PWR_TIMER0 = 1,
    CLKPWR_PWR_TIMER1 = 2,
    CLKPWR_PWR_UART0  = 3,
    CLKPWR_PWR_UART1  = 4,
    CLKPWR_PWR_PWM1   = 6,
    CLKPWR_PWR_I2C0   = 7,
    CLKPWR_PWR_SPI    = 8,
    CLKPWR_PWR_RTC    = 9,
    CLKPWR_PWR_RIT    = 16,
    CLKPWR_PWR_I2C1   = 19,
    CLKPWR_PWR_TIMER2 = 22,
    CLKPWR_PWR_UART2  = 24,
    CLKPWR_PWR_UART3  = 25
} CLKPWR_PeripheralPower;

/**
 * @brief Gets the core clock frequency.
 *
 * @return Core clock frequency in Hz.
 */
uint32_t CLKPWR_GetCClk(void);

/**
 * @brief Gets the peripheral clock frequency.
 *
 * @param periph Peripheral identifier.
 * @return Peripheral clock frequency in Hz.
 */
uint32_t CLKPWR_GetPClk(CLKPWR_PeripheralClock periph);

/**
 * @brief Sets the peripheral clock divider.
 *
 * @param periph Peripheral identifier.
 * @param div    Clock division factor.
 */
void CLKPWR_SetPClk(CLKPWR_PeripheralClock periph, CLKPWR_ClockDiv div);

/**
 * @brief Enables power for a peripheral.
 *
 * @param periph Peripheral identifier.
 */
void CLKPWR_Enable(CLKPWR_PeripheralPower periph);

/**
 * @brief Disables power for a peripheral.
 *
 * @param periph Peripheral identifier.
 */
void CLKPWR_Disable(CLKPWR_PeripheralPower periph);

/** @} */  // CLOCKPOWER
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* CLOCKPOWER_H_ */
