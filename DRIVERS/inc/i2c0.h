/**
 * @file i2c0.h
 * @brief I2C0 Driver API for LPC1769.
 *
 * This module provides a master-mode interface to the I2C peripheral.
 * It allows communication with external devices such as sensors, EEPROMs,
 * RTCs and displays.
 *
 * @version 1.0
 * @date 18 Jul 2025
 * @author PSampaio
 *
 * @note This software is provided for educational purposes.
 * @warning Provided "AS IS", without any warranties.
 */

#ifndef I2C0_H_
#define I2C0_H_

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
 * @defgroup I2C0 I2C0 — I2C Communication Driver
 * @brief Master-mode driver for the I2C peripheral.
 *
 * Supports start/stop sequences, byte transfers and block transfers.
 * @{
 */

/**
 * @brief I2C status codes (hardware state machine).
 */
typedef enum {
    // Master General
    I2C0_STARTED        = 0x08, /**< Start condition transmitted */
    I2C0_REPEATED_START = 0x10, /**< Repeated start transmitted */
    I2C0_SLA_W_ACK      = 0x18, /**< SLA+W acknowledged */
    I2C0_SLA_W_NACK     = 0x20, /**< SLA+W not acknowledged */
    I2C0_DATA_ACK        = 0x28, /**< Data byte acknowledged */
    I2C0_DATA_NACK       = 0x30, /**< Data byte not acknowledged */
    I2C0_SLA_R_ACK      = 0x40, /**< SLA+R acknowledged */
    I2C0_SLA_R_NACK     = 0x48, /**< SLA+R not acknowledged */
    I2C0_RX_DATA_ACK    = 0x50, /**< Data received, ACK sent */
    I2C0_RX_DATA_NACK   = 0x58  /**< Data received, NACK sent */
} I2C0_Status;

/**
 * @brief I2C driver error codes.
 */
typedef enum {
    I2C0_SUCCESS      = 0,   /**< Operation successful */
    I2C0_START_FAIL   = -1,  /**< Failed to generate START */
    I2C0_NACK_ADDRESS = -2,  /**< Address not acknowledged */
    I2C0_NACK_DATA    = -3   /**< Data not acknowledged */
} I2C0_ErrorCode;

/**
 * @brief Initializes the I2C peripheral.
 *
 * @note Must be called before any I2C communication.
 */
void I2C0_Init(void);

/**
 * @brief Sets the I2C bus frequency.
 *
 * @param freq Desired I2C clock frequency in Hz.
 */
void I2C0_SetFrequency(uint32_t freq);

/**
 * @brief Generates an I2C START condition.
 *
 * @retval true  START generated
 * @retval false START failed
 */
bool I2C0_Start(void);

/** @brief Generates an I2C STOP condition. */
void I2C0_Stop(void);

/**
 * @brief Writes a single byte.
 *
 * @param data Byte to transmit.
 * @retval true  Byte acknowledged
 * @retval false Byte not acknowledged
 */
bool I2C0_WriteByte(uint8_t data);

/**
 * @brief Reads a single byte.
 *
 * @param ack true to send ACK, false to send NACK.
 * @return Byte received.
 */
uint8_t I2C0_ReadByte(bool ack);

/**
 * @brief Writes a block of data to a slave device.
 *
 * @param address 7-bit I2C slave address.
 * @param data    Pointer to data buffer.
 * @param length  Number of bytes to send.
 * @return I2C0_SUCCESS or error code.
 */
I2C0_ErrorCode I2C0_Write(uint8_t address, const uint8_t *data, uint32_t length);

/**
 * @brief Reads a block of data from a slave device.
 *
 * @param address 7-bit I2C slave address.
 * @param buffer  Pointer to receive buffer.
 * @param length  Number of bytes to read.
 * @return I2C0_SUCCESS or error code.
 */
I2C0_ErrorCode I2C0_Read(uint8_t address, uint8_t *buffer, uint32_t length);

/**
 * @brief Reads a 16-bit register from a slave device.
 *
 * @param address 7-bit I2C slave address.
 * @param reg     Register address.
 * @param value   Pointer to store register value.
 * @return I2C0_SUCCESS or error code.
 */
I2C0_ErrorCode I2C0_ReadRegister(uint8_t address, uint8_t reg, uint16_t *value);

/** @} */  // I2C0
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* I2C0_H_ */
