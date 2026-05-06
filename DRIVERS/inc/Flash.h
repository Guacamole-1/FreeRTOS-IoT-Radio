/**
* @file Flash.h
* @brief API for writing/erasing/verifying internal Flash using LPC17xx IAP.
* @version 2.1
* @date 27 Nov 2025
* @author Grupo06
*/

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>

#define IAP_LOCATION    0x1FFF1FF1UL

/*
 * Sector used to store Radio_Data.
 * LPC1769 sector 29 starts at 0x00078000 and has 32 KB.
 */
#define SECTOR_POS      0x00078000UL
#define SECTOR          29U
#define SECTOR_SIZE     0x8000U

#define IAP_ERASE       52U
#define IAP_COPY        51U
#define IAP_PREP        50U
#define IAP_COMP        56U

typedef enum
{
    CMD_SUCCESS = 0,                /* Command executed successfully */
    INVALID_COMMAND,                /* Invalid command */
    SRC_ADDR_ERROR,                 /* Source address not on word boundary */
    DST_ADDR_ERROR,                 /* Destination address not on correct boundary */
    SRC_ADDR_NOT_MAPPED,            /* Source address not mapped in memory map */
    DST_ADDR_NOT_MAPPED,            /* Destination address not mapped in memory map */
    COUNT_ERROR,                    /* Byte count invalid */
    INVALID_SECTOR,                 /* Invalid sector number */
    SECTOR_NOT_BLANK,               /* Sector not blank */
    SECTOR_NOT_PREPARED_FOR_WRITE,  /* Sector not prepared for write operation */
    COMPARE_ERROR,                  /* Source and destination data not equal */
    BUSY                            /* Flash programming hardware is busy */
} CMD_RESULTS;

/**
 * @brief Erases one Flash sector.
 * @param sector sector number.
 * @return IAP result code.
 */
CMD_RESULTS FLASH_EraseSector(unsigned int sector);

/**
 * @brief Writes data block referenced by srcAddr into Flash at dstAddr.
 *
 * @note LPC17xx IAP copy command only supports sizes 256, 512, 1024 or 4096 bytes.
 *
 * @param dstAddr destination Flash address.
 * @param srcAddr source RAM address.
 * @param size number of bytes to write.
 * @return IAP result code.
 */
CMD_RESULTS FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size);

/**
 * @brief Compares data block referenced by srcAddr with Flash at dstAddr.
 * @param dstAddr destination Flash address.
 * @param srcAddr source address.
 * @param size number of bytes to compare.
 * @return IAP result code.
 */
CMD_RESULTS FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size);

#endif /* FLASH_H_ */
