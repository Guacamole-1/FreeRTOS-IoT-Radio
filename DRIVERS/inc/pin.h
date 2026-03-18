/**
 * @file pin.h
 * @brief GPIO (General Purpose I/O) Driver API for LPC1769.
 *
 * This module provides an interface to configure and control GPIO pins.
 * It supports per-pin and per-port access and GPIO interrupt handling.
 *
 * @version 1.0
 * @date 6 Nov 2018
 * @author PSampaio
 *
 * @note This software is provided for educational purposes.
 * @warning Provided "AS IS", without any warranties.
 */

#ifndef PIN_H_
#define PIN_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup DRIVERS
 * @{
 */

/**
 * @defgroup PIN PIN — GPIO Driver
 * @brief Driver to control LPC1769 GPIO pins.
 *
 * Provides pin direction control, read/write operations and interrupts.
 * @{
 */

/**
 * @brief GPIO port identifier.
 */
typedef enum {
    PIN_PORT0 = 0, /**< GPIO Port 0 */
    PIN_PORT1,     /**< GPIO Port 1 */
    PIN_PORT2,     /**< GPIO Port 2 */
    PIN_PORT3      /**< GPIO Port 3 */
} PIN_PortType;

/**
 * @brief GPIO pin identifier.
 *
 * Each value encodes the port and pin number as (port * 32 + pin).
 * UNDEF pins are reserved and may not be bonded or usable.
 */
typedef enum PIN_ValueEnum {
	// PORT 0
	GPIO0_0 = (0 * 32 + 0),
	GPIO0_1 = (0 * 32 + 1),
	GPIO0_2 = (0 * 32 + 2),
	GPIO0_3 = (0 * 32 + 3),
	GPIO0_4 = (0 * 32 + 4),
	GPIO0_5 = (0 * 32 + 5),
	GPIO0_6 = (0 * 32 + 6),
	GPIO0_7 = (0 * 32 + 7),
	GPIO0_8 = (0 * 32 + 8),
	GPIO0_9 = (0 * 32 + 9),
	GPIO0_10 = (0 * 32 + 10),
	GPIO0_11 = (0 * 32 + 11),
	GPIO0_12 = (0 * 32 + 12),
	GPIO0_13 = (0 * 32 + 13),
	GPIO0_14 = (0 * 32 + 14),
	GPIO0_15 = (0 * 32 + 15),
	GPIO0_16 = (0 * 32 + 16),
	GPIO0_17 = (0 * 32 + 17),
	GPIO0_18 = (0 * 32 + 18),
	GPIO0_19 = (0 * 32 + 19),
	GPIO0_20 = (0 * 32 + 20),
	GPIO0_21 = (0 * 32 + 21),
	GPIO0_22 = (0 * 32 + 22),
	GPIO0_23 = (0 * 32 + 23),
	GPIO0_24 = (0 * 32 + 24),
	GPIO0_25 = (0 * 32 + 25),
	GPIO0_26 = (0 * 32 + 26),
	GPIO0_27 = (0 * 32 + 27),
	GPIO0_28 = (0 * 32 + 28),
	GPIO0_29 = (0 * 32 + 29),
	GPIO0_30 = (0 * 32 + 30),
	GPIO0_31 = (0 * 32 + 31),

	TXD3 = (0 * 32 + 0),
	RXD3 = (0 * 32 + 1),
	SDA1 = (0 * 32 + 0),
	SCL1 = (0 * 32 + 1),
	TXD0 = (0 * 32 + 2),
	RXD0 = (0 * 32 + 3),

	UNDEF0_4 = (0 * 32 + 4),
	UNDEF0_5 = (0 * 32 + 5),
	UNDEF0_6 = (0 * 32 + 6),
	UNDEF0_7 = (0 * 32 + 7),
	UNDEF0_8 = (0 * 32 + 8),
	UNDEF0_9 = (0 * 32 + 9),
	TXD2 = (0 * 32 + 10),
	RXD2 = (0 * 32 + 11),
	SDA2 = (0 * 32 + 10),
	SCL2 = (0 * 32 + 11),
	UNDEF0_12 = (0 * 32 + 12),
	UNDEF0_13 = (0 * 32 + 13),
	UNDEF0_14 = (0 * 32 + 14),
	SCK = (0 * 32 + 15),
	TXD1_ALT = (0 * 32 + 15),
	RXD1_ALT = (0 * 32 + 16),
	MISO = (0 * 32 + 17),
	MOSI = (0 * 32 + 18),
	SDA1_ALT = (0 * 32 + 19),
	SCL1_ALT = (0 * 32 + 20),
	UNDEF0_21 = (0 * 32 + 21),
	UNDEF0_22 = (0 * 32 + 22),
	UNDEF0_23 = (0 * 32 + 23),
	AD0_0 = (0 * 32 + 23),
	UNDEF0_24 = (0 * 32 + 24),
	UNDEF0_25 = (0 * 32 + 25),
	UNDEF0_26 = (0 * 32 + 26),
	SDA0 = (0 * 32 + 27),
	SCL0 = (0 * 32 + 28),
	UNDEF0_29 = (0 * 32 + 29),
	UNDEF0_30 = (0 * 32 + 30),
	UNDEF0_31 = (0 * 32 + 31),

	// PORT 1
	GPIO1_0 = (1 * 32 + 0),
	GPIO1_1 = (1 * 32 + 1),
	GPIO1_2 = (1 * 32 + 2),
	GPIO1_3 = (1 * 32 + 3),
	GPIO1_4 = (1 * 32 + 4),
	GPIO1_5 = (1 * 32 + 5),
	GPIO1_6 = (1 * 32 + 6),
	GPIO1_7 = (1 * 32 + 7),
	GPIO1_8 = (1 * 32 + 8),
	GPIO1_9 = (1 * 32 + 9),
	GPIO1_10 = (1 * 32 + 10),
	GPIO1_11 = (1 * 32 + 11),
	GPIO1_12 = (1 * 32 + 12),
	GPIO1_13 = (1 * 32 + 13),
	GPIO1_14 = (1 * 32 + 14),
	GPIO1_15 = (1 * 32 + 15),
	GPIO1_16 = (1 * 32 + 16),
	GPIO1_17 = (1 * 32 + 17),
	GPIO1_18 = (1 * 32 + 18),
	GPIO1_19 = (1 * 32 + 19),
	GPIO1_20 = (1 * 32 + 20),
	GPIO1_21 = (1 * 32 + 21),
	GPIO1_22 = (1 * 32 + 22),
	GPIO1_23 = (1 * 32 + 23),
	GPIO1_24 = (1 * 32 + 24),
	GPIO1_25 = (1 * 32 + 25),
	GPIO1_26 = (1 * 32 + 26),
	GPIO1_27 = (1 * 32 + 27),
	GPIO1_28 = (1 * 32 + 28),
	GPIO1_29 = (1 * 32 + 29),
	GPIO1_30 = (1 * 32 + 30),
	GPIO1_31 = (1 * 32 + 31),

	UNDEF1_0 = (0 * 32 + 0),
	UNDEF1_1 = (0 * 32 + 1),
	UNDEF1_2 = (0 * 32 + 2),
	UNDEF1_3 = (0 * 32 + 3),
	UNDEF1_4 = (0 * 32 + 4),
	UNDEF1_5 = (0 * 32 + 5),
	UNDEF1_6 = (0 * 32 + 6),
	UNDEF1_7 = (0 * 32 + 7),
	UNDEF1_8 = (0 * 32 + 8),
	UNDEF1_9 = (0 * 32 + 9),
	UNDEF1_10 = (0 * 32 + 10),
	UNDEF1_11 = (0 * 32 + 11),
	UNDEF1_12 = (0 * 32 + 12),
	UNDEF1_13 = (0 * 32 + 13),
	UNDEF1_14 = (0 * 32 + 14),
	UNDEF1_15 = (0 * 32 + 15),
	UNDEF1_16 = (0 * 32 + 16),
	UNDEF1_17 = (0 * 32 + 17),
	UNDEF1_18 = (0 * 32 + 18),
	UNDEF1_19 = (0 * 32 + 19),
	UNDEF1_20 = (0 * 32 + 20),
	UNDEF1_21 = (0 * 32 + 21),
	UNDEF1_22 = (0 * 32 + 22),
	UNDEF1_23 = (0 * 32 + 23),
	UNDEF1_24 = (0 * 32 + 24),
	UNDEF1_25 = (0 * 32 + 25),
	UNDEF1_26 = (0 * 32 + 26),
	UNDEF1_27 = (0 * 32 + 27),
	UNDEF1_28 = (0 * 32 + 28),
	UNDEF1_29 = (0 * 32 + 29),
	UNDEF1_30 = (0 * 32 + 30),
	UNDEF1_31 = (0 * 32 + 31),

	// PORT 2
	GPIO2_0 = (2 * 32 + 0),
	GPIO2_1 = (2 * 32 + 1),
	GPIO2_2 = (2 * 32 + 2),
	GPIO2_3 = (2 * 32 + 3),
	GPIO2_4 = (2 * 32 + 4),
	GPIO2_5 = (2 * 32 + 5),
	GPIO2_6 = (2 * 32 + 6),
	GPIO2_7 = (2 * 32 + 7),
	GPIO2_8 = (2 * 32 + 8),
	GPIO2_9 = (2 * 32 + 9),
	GPIO2_10 = (2 * 32 + 10),
	GPIO2_11 = (2 * 32 + 11),
	GPIO2_12 = (2 * 32 + 12),
	GPIO2_13 = (2 * 32 + 13),
	GPIO2_14 = (2 * 32 + 14),
	GPIO2_15 = (2 * 32 + 15),
	GPIO2_16 = (2 * 32 + 16),
	GPIO2_17 = (2 * 32 + 17),
	GPIO2_18 = (2 * 32 + 18),
	GPIO2_19 = (2 * 32 + 19),
	GPIO2_20 = (2 * 32 + 20),
	GPIO2_21 = (2 * 32 + 21),
	GPIO2_22 = (2 * 32 + 22),
	GPIO2_23 = (2 * 32 + 23),
	GPIO2_24 = (2 * 32 + 24),
	GPIO2_25 = (2 * 32 + 25),
	GPIO2_26 = (2 * 32 + 26),
	GPIO2_27 = (2 * 32 + 27),
	GPIO2_28 = (2 * 32 + 28),
	GPIO2_29 = (2 * 32 + 29),
	GPIO2_30 = (2 * 32 + 30),
	GPIO2_31 = (2 * 32 + 31),

	TXD1 = (2 * 32 + 0),
	RXD1 = (2 * 32 + 1),
	UNDEF2_2 = (0 * 32 + 2),
	UNDEF2_3 = (0 * 32 + 3),
	UNDEF2_4 = (0 * 32 + 4),
	PWM1_6 = (2 * 32 + 5),
	UNDEF2_6 = (0 * 32 + 6),
	UNDEF2_7 = (0 * 32 + 7),
	UNDEF2_8 = (0 * 32 + 8),
	UNDEF2_9 = (0 * 32 + 9),
	UNDEF2_10 = (0 * 32 + 10),
	UNDEF2_11 = (0 * 32 + 11),
	UNDEF2_12 = (0 * 32 + 12),
	UNDEF2_13 = (0 * 32 + 13),
	UNDEF2_14 = (0 * 32 + 14),
	UNDEF2_15 = (0 * 32 + 15),
	UNDEF2_16 = (0 * 32 + 16),
	UNDEF2_17 = (0 * 32 + 17),
	UNDEF2_18 = (0 * 32 + 18),
	UNDEF2_19 = (0 * 32 + 19),
	UNDEF2_20 = (0 * 32 + 20),
	UNDEF2_21 = (0 * 32 + 21),
	UNDEF2_22 = (0 * 32 + 22),
	UNDEF2_23 = (0 * 32 + 23),
	UNDEF2_24 = (0 * 32 + 24),
	UNDEF2_25 = (0 * 32 + 25),
	UNDEF2_26 = (0 * 32 + 26),
	UNDEF2_27 = (0 * 32 + 27),
	UNDEF2_28 = (0 * 32 + 28),
	UNDEF2_29 = (0 * 32 + 29),
	UNDEF2_30 = (0 * 32 + 30),
	UNDEF2_31 = (0 * 32 + 31),

	// PORT 3
	GPIO3_0 = (3 * 32 + 0),
	GPIO3_1 = (3 * 32 + 1),
	GPIO3_2 = (3 * 32 + 2),
	GPIO3_3 = (3 * 32 + 3),
	GPIO3_4 = (3 * 32 + 4),
	GPIO3_5 = (3 * 32 + 5),
	GPIO3_6 = (3 * 32 + 6),
	GPIO3_7 = (3 * 32 + 7),
	GPIO3_8 = (3 * 32 + 8),
	GPIO3_9 = (3 * 32 + 9),
	GPIO3_10 = (3 * 32 + 10),
	GPIO3_11 = (3 * 32 + 11),
	GPIO3_12 = (3 * 32 + 12),
	GPIO3_13 = (3 * 32 + 13),
	GPIO3_14 = (3 * 32 + 14),
	GPIO3_15 = (3 * 32 + 15),
	GPIO3_16 = (3 * 32 + 16),
	GPIO3_17 = (3 * 32 + 17),
	GPIO3_18 = (3 * 32 + 18),
	GPIO3_19 = (3 * 32 + 19),
	GPIO3_20 = (3 * 32 + 20),
	GPIO3_21 = (3 * 32 + 21),
	GPIO3_22 = (3 * 32 + 22),
	GPIO3_23 = (3 * 32 + 23),
	GPIO3_24 = (3 * 32 + 24),
	GPIO3_25 = (3 * 32 + 25),
	GPIO3_26 = (3 * 32 + 26),
	GPIO3_27 = (3 * 32 + 27),
	GPIO3_28 = (3 * 32 + 28),
	GPIO3_29 = (3 * 32 + 29),
	GPIO3_30 = (3 * 32 + 30),
	GPIO3_31 = (3 * 32 + 31),

	UNDEF3_0 = (0 * 32 + 0),
	UNDEF3_1 = (0 * 32 + 1),
	UNDEF3_2 = (0 * 32 + 2),
	UNDEF3_3 = (0 * 32 + 3),
	UNDEF3_4 = (0 * 32 + 4),
	UNDEF3_5 = (0 * 32 + 5),
	UNDEF3_6 = (0 * 32 + 6),
	UNDEF3_7 = (0 * 32 + 7),
	UNDEF3_8 = (0 * 32 + 8),
	UNDEF3_9 = (0 * 32 + 9),
	UNDEF3_10 = (0 * 32 + 10),
	UNDEF3_11 = (0 * 32 + 11),
	UNDEF3_12 = (0 * 32 + 12),
	UNDEF3_13 = (0 * 32 + 13),
	UNDEF3_14 = (0 * 32 + 14),
	UNDEF3_15 = (0 * 32 + 15),
	UNDEF3_16 = (0 * 32 + 16),
	UNDEF3_17 = (0 * 32 + 17),
	UNDEF3_18 = (0 * 32 + 18),
	UNDEF3_19 = (0 * 32 + 19),
	UNDEF3_20 = (0 * 32 + 20),
	UNDEF3_21 = (0 * 32 + 21),
	UNDEF3_22 = (0 * 32 + 22),
	UNDEF3_23 = (0 * 32 + 23),
	UNDEF3_24 = (0 * 32 + 24),
	UNDEF3_25 = (0 * 32 + 25),
	UNDEF3_26 = (0 * 32 + 26),
	UNDEF3_27 = (0 * 32 + 27),
	UNDEF3_28 = (0 * 32 + 28),
	UNDEF3_29 = (0 * 32 + 29),
	UNDEF3_30 = (0 * 32 + 30),
	UNDEF3_31 = (0 * 32 + 31)
} PIN_ValueType;

/**
 * @brief GPIO pin direction.
 */
typedef enum {
    PIN_INPUT,   /**< Pin configured as input */
    PIN_OUTPUT   /**< Pin configured as output */
} PIN_DirectionEnumType;

/**
 * @brief GPIO interrupt trigger type.
 */
typedef enum {
    PIN_INT_RISING = 1,          /**< Trigger on rising edge */
    PIN_INT_FALLING = 2,         /**< Trigger on falling edge */
    PIN_INT_BOTH = 3             /**< Trigger on both edges */
} PIN_InterruptEdgeEnumType;

/**
 * @brief GPIO interrupt handler prototype.
 */
typedef void (*PIN_HandlerType)(void);

/**
 * @brief Sets the direction of a GPIO pin.
 *
 * @param pinId Pin identifier.
 * @param dir   Direction (input or output).
 */
void PIN_Direction(PIN_ValueType pinId, PIN_DirectionEnumType dir);

/**
 * @brief Sets a GPIO pin to logic high.
 *
 * @param pinId Pin identifier.
 */
void PIN_Set(PIN_ValueType pinId);

/**
 * @brief Sets a GPIO pin to logic low.
 *
 * @param pinId Pin identifier.
 */
void PIN_Clear(PIN_ValueType pinId);

/**
 * @brief Reads the current value of a GPIO pin.
 *
 * @param pinId Pin identifier.
 * @retval true  Pin is high
 * @retval false Pin is low
 */
bool PIN_Value(PIN_ValueType pinId);

/**
 * @brief Writes a value to an entire GPIO port.
 *
 * @param portId GPIO port.
 * @param value  Bitmask to write.
 */
void PIN_PortSet(PIN_PortType portId, uint32_t value);

/**
 * @brief Clears selected bits of a GPIO port.
 *
 * @param portId GPIO port.
 * @param value  Bitmask of bits to clear.
 */
void PIN_PortClear(PIN_PortType portId, uint32_t value);

/**
 * @brief Writes masked values to a GPIO port.
 *
 * @param portId GPIO port.
 * @param value  Bits to write.
 * @param mask   Mask selecting which bits are affected.
 */
void PIN_PortWrite(PIN_PortType portId, uint32_t value, uint32_t mask);

/**
 * @brief Reads the current value of a GPIO port.
 *
 * @param portId GPIO port.
 * @return Current port value.
 */
uint32_t PIN_PortRead(PIN_PortType portId);

/**
 * @brief Registers an interrupt handler for a GPIO pin.
 *
 * @param pinId   Pin identifier.
 * @param type    Interrupt edge type.
 * @param handler Function to call when interrupt occurs.
 */
void PIN_SetHandler(PIN_ValueType pinId,
                    PIN_InterruptEdgeEnumType type,
                    PIN_HandlerType handler);

/**
 * @brief Gets the interrupt cause for a GPIO pin.
 *
 * @param pinId Pin identifier.
 * @return Interrupt edge that triggered.
 */
PIN_InterruptEdgeEnumType PIN_GetIntrCause(PIN_ValueType pinId);

/**
 * @brief Clears the interrupt flag of a GPIO pin.
 *
 * @param pinId Pin identifier.
 */
void PIN_ClearIntr(PIN_ValueType pinId);

/** @} */  // PIN
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* PIN_H_ */
