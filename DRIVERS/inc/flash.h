/**
 * @file flash.h
 * @brief Internal FLASH Memory Driver API for LPC1769.
 *
 * This module provides an interface to the internal FLASH memory.
 * It supports sector preparation, erase operations, data writing and verification.
 *
 * @version 1.0
 * @date 2025
 * @author PSampaio
 *
 * @note This software is provided for educational purposes.
 * @warning Incorrect FLASH operations may corrupt program memory.
 */

#ifndef FLASH_H_
#define FLASH_H_

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
 * @defgroup FLASH FLASH — Internal Flash Memory Driver
 * @brief Driver to manage LPC1769 internal FLASH memory.
 *
 * Provides safe access to FLASH sectors through the IAP interface.
 * @{
 */

/**
 * @brief FLASH controller status codes returned by IAP commands.
 */
typedef enum {
    FLASH_CMD_SUCCESS = 0,      /**< Command executed successfully */
    FLASH_INVALID_CMD,          /**< Invalid IAP command */
    FLASH_SRC_ADDR_ERROR,       /**< Source address not on a word boundary */
    FLASH_DST_ADDR_ERROR,       /**< Destination address not on a sector boundary */
    FLASH_SRC_ADDR_NOT_MAPPED,  /**< Source address outside valid memory region */
    FLASH_DST_ADDR_NOT_MAPPED,  /**< Destination address outside FLASH memory */
    FLASH_COUNT_ERROR,          /**< Byte count is not a valid size */
    FLASH_INVALID_SECTOR,       /**< Sector number out of range */
    FLASH_SECTOR_NOT_BLANK,     /**< Sector is not blank before write */
    FLASH_SECTOR_NOT_PREPARED,  /**< Sector was not prepared before write/erase */
    FLASH_COMPARE_ERROR,        /**< FLASH content does not match source buffer */
    FLASH_BUSY                  /**< FLASH controller busy executing a command */
} FLASH_Status;


/**
 * @brief Prepares a FLASH sector for erase or write.
 *
 * @param sector Sector number.
 * @return FLASH status code.
 */
FLASH_Status FLASH_PrepareSector(uint32_t sector);

/**
 * @brief Erases a FLASH sector.
 *
 * @param sector Sector number.
 * @return FLASH status code.
 */
FLASH_Status FLASH_EraseSector(uint32_t sector);

/**
 * @brief Writes a block of data to FLASH.
 *
 * @param dstAddr Destination FLASH address.
 * @param srcAddr Source RAM buffer.
 * @param size    Data size in bytes (must meet alignment requirements).
 *
 * @note The sector must be prepared before writing.
 *
 * @return FLASH status code.
 */
FLASH_Status FLASH_WriteData(void *dstAddr, const void *srcAddr, uint32_t size);

/**
 * @brief Verifies FLASH content against a RAM buffer.
 *
 * @param dstAddr FLASH address.
 * @param srcAddr RAM buffer.
 * @param size    Data size in bytes.
 *
 * @return FLASH status code.
 */
FLASH_Status FLASH_VerifyData(const void *dstAddr, const void *srcAddr, uint32_t size);

/** @} */  // FLASH
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* FLASH_H_ */
