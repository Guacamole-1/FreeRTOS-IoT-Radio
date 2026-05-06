/*
 * Radio.c
 *
 *  Created on: 9 Dec 2025
 *      Author: mcdnv
 *
 *  Updated:
 *  - Volume/channel changes no longer save automatically to Flash.
 *  - Flash save is only done through Save_RData(), called from explicit Save menu.
 */

#include "I2C.h"
#include "Radio.h"
#include "Flash.h"

#include <stdint.h>
#include <string.h>

#define RADIO_FLASH_WRITE_SIZE  256U

Radio_State Init_Radio(Radio_Data* radio_data) {
    Radio_State tmp;
    Radio_Data temp = {0};

    DELAY_Init();
    I2CMASTER_Init();

    /*
     * Try to load saved data from Flash.
     * If no valid saved data exists, use the defaults provided by the app.
     * IMPORTANT:
     * We do NOT save defaults automatically. Save is now explicit from menu.
     */
    if (Get_RData(&temp) != CMD_SUCCESS) {
        if (radio_data != NULL) {
            temp = *radio_data;
        }
    } else if (radio_data != NULL) {
        *radio_data = temp;
    }

    int regs_size = 3;
    int regs[] = {2, 3, 5};
    uint16_t data[] = {
        DHIZ | DMUTE | NEW_METHOD | POWER_UP,                 /* REGISTER 2 */
        SPACE(0) | BAND | TUNE | CHAN(temp.channel),          /* REGISTER 3 */
        VOLUME(temp.volume) | LNA_PORT | SEEK_TH | INT_MODE   /* REGISTER 5 */
    };

    for (int i = 0; i < regs_size; i++) {
        if ((tmp = Radio_setReg(regs[i], data[i], NULL)) != Sucess) {
            return tmp;
        }
    }

    return Sucess;
}

Radio_State Set_Volume(Radio_Data* Rdata, I2C_MState* return_state) {
    uint16_t data;

    if (Rdata == NULL) {
        return Flash_error;
    }

    data = VOLUME(Rdata->volume) | LNA_PORT | SEEK_TH | INT_MODE;

    /*
     * Only apply volume to the radio chip.
     * Do NOT save to Flash here.
     */
    return Radio_setReg(5, data, return_state);
}

Radio_State Set_Channel(Radio_Data* Rdata, I2C_MState* return_state) {
    uint16_t data;

    if (Rdata == NULL) {
        return Flash_error;
    }

    data = SPACE(0) | BAND | TUNE | CHAN(Rdata->channel);

    /*
     * Only apply channel/frequency to the radio chip.
     * Do NOT save to Flash here.
     */
    return Radio_setReg(3, data, return_state);
}

CMD_RESULTS Save_RData(Radio_Data* data) {
    CMD_RESULTS result;
    Radio_Data save_data;

    /*
     * 256 bytes is the minimum valid IAP copy size.
     * uint32_t guarantees word alignment for IAP.
     */
    uint32_t flash_buffer[RADIO_FLASH_WRITE_SIZE / sizeof(uint32_t)];

    if (data == NULL) {
        return INVALID_COMMAND;
    }

    memset(&save_data, 0, sizeof(save_data));
    save_data = *data;

    /*
     * Force the identifier before saving, so even if the caller forgot to set it,
     * Flash still receives a valid record.
     */
    memset(save_data.id, 0, sizeof(save_data.id));
    strncpy(save_data.id, RADIO_DATA_ID, sizeof(save_data.id) - 1U);

    memset(flash_buffer, 0xFF, sizeof(flash_buffer));
    memcpy(flash_buffer, &save_data, sizeof(save_data));

    result = FLASH_EraseSector(SECTOR);
    if (result != CMD_SUCCESS) {
        return result;
    }

    result = FLASH_WriteData((void*)SECTOR_POS,
                             (void*)flash_buffer,
                             sizeof(flash_buffer));
    if (result != CMD_SUCCESS) {
        return result;
    }

    result = FLASH_VerifyData((void*)SECTOR_POS,
                              (void*)flash_buffer,
                              sizeof(flash_buffer));
    if (result != CMD_SUCCESS) {
        return result;
    }

    return CMD_SUCCESS;
}

CMD_RESULTS Get_RData(Radio_Data* data) {
    Radio_Data temp;

    if (data == NULL) {
        return INVALID_COMMAND;
    }

    /*
     * Flash is memory-mapped, so reading can be done directly with memcpy.
     * This avoids using IAP COMP for a short string, which can be fragile with
     * alignment/size constraints.
     */
    memcpy(&temp, (const void*)SECTOR_POS, sizeof(temp));

    /*
     * Check if the saved record is valid.
     * If Flash is blank, temp.id will usually be 0xFF and this comparison fails.
     */
    if (strncmp(temp.id, RADIO_DATA_ID, sizeof(temp.id)) != 0) {
        return COMPARE_ERROR;
    }

    /*
     * Basic sanity checks to avoid loading corrupted values.
     */
    if (temp.volume > 15U) {
        return COMPARE_ERROR;
    }

    if (temp.channel > 1023U) {
        return COMPARE_ERROR;
    }

    *data = temp;

    return CMD_SUCCESS;
}

/* Get Radio Register */
Radio_State Radio_getReg(uint8_t reg, uint16_t* register_data, I2C_MState* return_state) {
    uint8_t buf[2];
    I2C_MState tmp;

    tmp = I2CMASTER_Transmit(RADIO_ADDR, &reg, 1);
    if (return_state) {
        *return_state = tmp;
    }

    if (tmp != TX_data_ACK && tmp != TX_data) {
        return Reg_select_error;
    }

    tmp = I2CMASTER_Receive(RADIO_ADDR, buf, 2);
    if (return_state) {
        *return_state = tmp;
    }

    if (tmp != RX_data_NACK && tmp != RX_data) {
        return Data_Receive_error;
    }

    if (register_data) {
        *register_data = ((uint16_t)buf[0] << 8) | buf[1];
    }

    return Sucess;
}

Radio_State Radio_setReg(uint8_t reg, uint16_t data, I2C_MState* return_state) {
    I2C_MState tmp;
    int data_size = 3;
    uint8_t sdata[3];

    sdata[0] = reg;
    sdata[1] = data >> 8;
    sdata[2] = data & 0xFF;

    tmp = I2CMASTER_Transmit(RADIO_ADDR, sdata, data_size);
    if (return_state) {
        *return_state = tmp;
    }

    if (tmp != TX_data_ACK && tmp != TX_data) {
        return Data_Transmit_error;
    }

    return Sucess;
}
