/**
* @file I2C.h
* @brief This header file provides the API for the I2C controller
* @version 1.1
* @date 27 Nov 2025
* @author Grupo06
*
*/
/**
* @addtogroup DRIVERS
* @{
*/

/**
* @defgroup I2C I2C functions
* @brief This module provides core functions for I2C
* @{
*/

#ifndef I2C_H_
#define I2C_H_


//pag 439 442
#include "LPC17xx.h"
#include "Delay.h"
#include <stdbool.h>


typedef enum{
    f100k, f400k, f1M, Invalid_freq
} I2C_Freq;

typedef enum{
    None        = 0,
// --- Master states ---
    Start_tx    = 0x08, // A START condition has been transmitted.
    Start_Rtx   = 0x10, // A repeated START condition has been transmitted.
// --- Master Transmitter states ---
    TX_data     = 0x18, // Previous state was State 0x08 or 0x10 and ACK has been received. The first data byte will be transmitted.
    Addr_NACK   = 0x20, // Slave Address + Write has been transmitted, NOT ACK has been received. A STOP condition will be transmitted.
    TX_data_ACK = 0x28, // Data has been transmitted, ACK has been received.
    TX_data_NACK= 0x30, // Data has been transmitted, NOT ACK received
    Backed_off  = 0x38, // Arbitration has been lost during Slave Address + Write or data, A new START condition will be transmitted when the bus is free again.
// --- Master Receive states ---
    Request_RX  = 0x40, // Slave Address + Read has been transmitted, ACK has been received. Data will be received and ACK returned.
    Request_RX_NACK= 0x48,//Slave Address + Read has been transmitted, NOT ACK has been received
    RX_data     = 0x50,  // Data has been received, returns NACK if it is the last byte, otherwise returns ACK
    RX_data_NACK= 0x58  // Data has been received, NOT ACK has been returned
} I2C_MState; // I2C master states


typedef struct
{
    uint8_t* data;
    uint8_t slave_addr;
    bool R_W;           // Read = 1, Write = 0
    int data_size;
    int data_index;
    I2C_MState state;

}I2C_SM; //I2C struct for the state machine



/**
 * @brief Starts I2C0 controller and configures its respective pins
 * @warning  DELAY_Init must be called before using this function!
 */
void I2CMASTER_Init(void);


/**
 * @brief Configures I2C frequency
 * @param frequency f100k, f400k, f1M
 */
I2C_Freq I2CMASTER_SetFrequency(I2C_Freq frequency);
/* Realiza a transmissão dos dados referenciados por data com dimensão size bytes
para o dispositivo com o endereço devAddress. Retorna sucesso ou o erro ocorrido. */

/**
 * @brief Transmits size bytes of data to I2C device devAddress
 * @returns Final I2C state that caused stoppage
 */
I2C_MState I2CMASTER_Transmit(uint8_t devAddress, uint8_t *data, unsigned int size);


/**
 * @brief Receives size bytes of data from I2C device devAddress
 * @returns Final I2C state that caused stoppage
 */
I2C_MState I2CMASTER_Receive(uint8_t devAddress, uint8_t *data, unsigned int size);



#endif /* I2C_H_ */

/**
* @}
*/

/**
* @}
*/
