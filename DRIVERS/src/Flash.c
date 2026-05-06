/**
* @file Flash.c
* @brief Internal Flash driver using LPC17xx IAP.
* @version 2.1
* @date 27 Nov 2025
* @author Grupo06
*/

#include "Flash.h"

#include "LPC17xx.h"
#include "system_LPC17xx.h"

typedef void (*IAP_Function)(uint32_t command[], uint32_t result[]);
static const IAP_Function iap_entry = (IAP_Function)IAP_LOCATION;

static void FLASH_CallIAP(uint32_t command[], uint32_t result[]) {
    /*
     * IAP erase/write runs from ROM and manipulates Flash.
     * Disable interrupts so no interrupt handler stored in Flash runs during IAP.
     */
    __disable_irq();
    iap_entry(command, result);
    __enable_irq();
}

static uint32_t FLASH_GetIAPClockKHz(void) {
    SystemCoreClockUpdate();
    return SystemCoreClock / 1000U;
}

static int FLASH_IsValidIAPWriteSize(unsigned int size) {
    return (size == 256U) ||
           (size == 512U) ||
           (size == 1024U) ||
           (size == 4096U);
}

CMD_RESULTS FLASH_EraseSector(unsigned int sector) {
    uint32_t command[5] = {0U};
    uint32_t result[5] = {0U};

    command[0] = IAP_PREP;
    command[1] = sector;
    command[2] = sector;
    FLASH_CallIAP(command, result);

    if (result[0] != CMD_SUCCESS) {
        return (CMD_RESULTS)result[0];
    }

    command[0] = IAP_ERASE;
    command[1] = sector;
    command[2] = sector;
    command[3] = FLASH_GetIAPClockKHz();
    FLASH_CallIAP(command, result);

    return (CMD_RESULTS)result[0];
}

CMD_RESULTS FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size) {
    uint32_t command[5] = {0U};
    uint32_t result[5] = {0U};

    if ((dstAddr == 0) || (srcAddr == 0)) {
        return INVALID_COMMAND;
    }

    if (!FLASH_IsValidIAPWriteSize(size)) {
        return COUNT_ERROR;
    }

    command[0] = IAP_PREP;
    command[1] = SECTOR;
    command[2] = SECTOR;
    FLASH_CallIAP(command, result);

    if (result[0] != CMD_SUCCESS) {
        return (CMD_RESULTS)result[0];
    }

    command[0] = IAP_COPY;
    command[1] = (uint32_t)dstAddr;
    command[2] = (uint32_t)srcAddr;
    command[3] = size;
    command[4] = FLASH_GetIAPClockKHz();
    FLASH_CallIAP(command, result);

    return (CMD_RESULTS)result[0];
}

CMD_RESULTS FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size) {
    uint32_t command[5] = {0U};
    uint32_t result[5] = {0U};

    if ((dstAddr == 0) || (srcAddr == 0) || (size == 0U)) {
        return INVALID_COMMAND;
    }

    command[0] = IAP_COMP;
    command[1] = (uint32_t)dstAddr;
    command[2] = (uint32_t)srcAddr;
    command[3] = size;
    FLASH_CallIAP(command, result);

    return (CMD_RESULTS)result[0];
}
