/**
 * @file uart.h
 * @brief UART (Universal Asynchronous Receiver/Transmitter) Driver API for LPC1769.
 *
 * This module provides a high-level interface to configure and use the UART
 * peripheral for serial communication. It allows initialization, transmission,
 * reception and formatted output.
 *
 * @version 1.0
 * @date 22 May 2012
 * @author PSampaio
 *
 * @note This software is provided for educational purposes.
 * @warning Provided "AS IS", without any warranties.
 */

#ifndef UART_H_
#define UART_H_

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
 * @defgroup UART UART — Serial Communication Driver
 * @brief UART communication driver for LPC1769.
 *
 * This module abstracts the hardware registers of UART0 and provides a simple API
 * for serial communication.
 *
 * ### Hardware connections
 *
 * UART0 Signal | LPC1769 Pin
 * ------------ | -----------
 * TXD0         | P0.2
 * RXD0         | P0.3
 *
 * @{
 */

/**
 * @brief Initializes the UART peripheral.
 *
 * Configures the UART with the specified baud rate and enables transmission
 * and reception.
 *
 * @param baud Desired baud rate (e.g., 9600, 115200).
 * @return true if initialization was successful, false otherwise.
 */
bool UART_Initialize(unsigned int baud);

/**
 * @brief Checks if a character is available in the receive buffer.
 *
 * @retval true  Character available
 * @retval false No data available
 */
bool UART_IsChar(void);

/**
 * @brief Peeks at the next received character without removing it.
 *
 * @param ch Pointer where the peeked character will be stored.
 * @retval true  Character available
 * @retval false No data available
 */
bool UART_PeekChar(unsigned char *ch);

/**
 * @brief Reads and removes a character from the receive buffer.
 *
 * @return The received character.
 */
unsigned char UART_ReadChar(void);

/**
 * @brief Reads a character from the receive buffer.
 *
 * @param ch Pointer where the received character will be stored.
 * @retval true  Character read
 * @retval false No data available
 */
bool UART_GetChar(unsigned char *ch);

/**
 * @brief Reads multiple bytes from the receive buffer.
 *
 * @param rxbuf Pointer to buffer where received data will be stored.
 * @param buflen Maximum number of bytes to read.
 * @return Number of bytes actually read.
 */
uint32_t UART_ReadBuffer(uint8_t *rxbuf, uint32_t buflen);

/**
 * @brief Transmits a single character.
 *
 * @param ch Character to transmit.
 * @retval true  Character queued for transmission
 * @retval false Transmission buffer full
 */
bool UART_WriteChar(unsigned char ch);

/**
 * @brief Transmits a null-terminated string.
 *
 * @param str Pointer to string to transmit.
 * @return Number of characters transmitted.
 */
uint32_t UART_WriteString(const char *str);

/**
 * @brief Transmits a block of data.
 *
 * @param buffer Pointer to data buffer.
 * @param len Number of bytes to transmit.
 * @return Number of bytes transmitted.
 */
uint32_t UART_WriteBuffer(const uint8_t *buffer, uint32_t len);

/**
 * @brief Writes a formatted string using printf-style formatting.
 *
 * @param format Format string.
 * @param ... Additional arguments.
 * @return Number of characters transmitted.
 *
 * @note Requires standard C library support.
 */
int UART_Printf(const char *format, ...);

/**
 * @brief Clears the UART receive buffer.
 */
void UART_FlushRx(void);

/**
 * @brief Clears the UART transmit buffer.
 */
void UART_FlushTx(void);

/** @} */  // UART
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* UART_H_ */
