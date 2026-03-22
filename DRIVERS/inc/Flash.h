/**
* @file Flash.h
* @brief This header file provides the API for controlling the LCD
* @version 2
* @date 27 Nov 2025
* @author Grupo06
*
*/

/**
* @addtogroup DRIVERS
* @{
*/

/**
* @defgroup Flash Flash functions
* @brief This module provides core functions the Flash memory
* @{
*/

#ifndef FLASH_H_
#define FLASH_H_

#define IAP_LOCATION 0x1FFF1FF1

typedef void (*IAP)(unsigned int [],unsigned int[]);
static IAP iap_entry=(IAP) IAP_LOCATION;

#define SECTOR_POS 0x00078000
#define SECTOR 29
#define SECTOR_SIZE 0x80000
#define IAP_ERASE 52
#define IAP_COPY 51
#define IAP_PREP 50
#define IAP_COMP 56

typedef enum
{
    CMD_SUCCESS,                    // Command executed successfully
    INVALID_COMMAND,                // Invalid command
    SRC_ADDR_ERROR,                 // Source address not on word boundary
    DST_ADDR_ERROR,                 // Destination address not on correct boundary
    SRC_ADDR_NOT_MAPPED,            // Source address not mapped in memory map
    DST_ADDR_NOT_MAPPED,            // Destination address not mapped in memory map
    COUNT_ERROR,                    // Byte count invalid (not multiple of 4 or not permitted)
    INVALID_SECTOR,                 // Invalid sector number
    SECTOR_NOT_BLANK,               // Sector not blank
    SECTOR_NOT_PREPARED_FOR_WRITE,  // Sector not prepared for write operation
    COMPARE_ERROR,                  // Source and destination data not equal
    BUSY						    // Flash programming hardware is busy
}CMD_RESULTS;

/**
* @brief Erases Flash Sector
* @param sector sector number (0-29)
* @returns result code
*/
CMD_RESULTS FLASH_EraseSector(unsigned int sector);


/**
* @brief writes data block with size bytes referenced by srcAddr at destination dstAddr
* @returns result code
*/
CMD_RESULTS FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size);

/* Compara o conteúdo do bloco de dados referenciado por srcAddr, de dimensão size bytes,
com o conteúdo do bloco de dados referenciado por dstAddr. */
/**
* @brief compares data block with size bytes referenced by srcAddr with destination dstAddr
* @returns result code
*/
CMD_RESULTS FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size);

#endif /* FLASH_H_ */

/**
* @}
*/

/**
* @}
*/
