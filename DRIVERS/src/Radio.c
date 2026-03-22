/*
 * Radio.c
 *
 *  Created on: 9 Dec 2025
 *      Author: mcdnv
 */
#include "I2C.h"
#include "Radio.h"
#include "Flash.h"
#include <string.h>




Radio_State Init_Radio(Radio_Data* radio_data){
    Radio_State tmp;
    DELAY_Init();
    I2CMASTER_Init();
    Radio_Data temp = {0};
    // get saved data from flash, if there isnt, create it 
    if (Get_RData(&temp) != CMD_SUCCESS && radio_data != NULL )
    {
        temp = *radio_data;
        Save_RData(radio_data);
    }else if (radio_data != NULL)
    {
        *radio_data = temp;
    }
    
    int regs_size = 3;
    int regs[] = {2,3,5};
    uint16_t data[] = {
        DHIZ | DMUTE | NEW_METHOD | POWER_UP ,                       // REGISTER 2
        SPACE(0) | BAND | TUNE | CHAN(temp.channel),         // REGISTER 3
        VOLUME(temp.volume) | LNA_PORT | SEEK_TH | INT_MODE  // REGISTER 5
    };

    for (int i = 0; i < regs_size; i++)
    {
        if((tmp = Radio_setReg(regs[i],data[i],NULL))){
        return tmp;
    }
    }
    
    return Sucess;   
}

Radio_State Set_Volume(Radio_Data* Rdata,I2C_MState* return_state){

    uint16_t data = VOLUME(Rdata->volume) | LNA_PORT | SEEK_TH | INT_MODE;
    Radio_State st = Radio_setReg(5,data,return_state);
    if (st != Sucess){
        return st;
    }else{
        if(Save_RData(Rdata) != CMD_SUCCESS){
            return Flash_error;
        }
    }
    return Sucess;
}

Radio_State Set_Channel(Radio_Data* Rdata, I2C_MState* return_state){

    uint16_t data = SPACE(0) | BAND | TUNE | CHAN(Rdata->channel);
    Radio_State st = Radio_setReg(3,data,return_state);
    return st;
}
CMD_RESULTS Save_RData(Radio_Data* data){
    if (data == NULL)
    {
        return -1;
    }
    
    char temp[256] = {0};
    memcpy(temp,(uint8_t*)data,sizeof(*data));
    FLASH_EraseSector(SECTOR);
    return FLASH_WriteData((void*)SECTOR_POS,temp,sizeof(temp));
}

CMD_RESULTS Get_RData(Radio_Data* data){
    if (data == NULL)
    {
        return -1;
    }
    CMD_RESULTS res;
    char * id = RADIO_DATA_ID;
    if ((res = FLASH_VerifyData((void*)SECTOR_POS,id,strlen(id)+1)) != CMD_SUCCESS)
    {
        return res;
    }
    memcpy(data,(void*)SECTOR_POS,sizeof(Radio_Data));

    return CMD_SUCCESS;
}



// Get Radio Register
Radio_State Radio_getReg(uint8_t reg, uint16_t* register_data, I2C_MState* return_state){
    uint8_t buf[2];
    I2C_MState tmp;


    tmp = I2CMASTER_Transmit(RADIO_ADDR,&reg,1);
    if (return_state) *return_state = tmp;

    if (tmp != TX_data_ACK && tmp != TX_data)
    {
        return Reg_select_error;
    }
    
    tmp = I2CMASTER_Receive(RADIO_ADDR, buf, 2);
    if (return_state) *return_state = tmp;

    if (tmp != RX_data_NACK && tmp != RX_data)
    {
        return Data_Receive_error;
    }
    if(register_data) *register_data = ((uint16_t)buf[0] << 8) | buf[1];

    return Sucess;

}

Radio_State Radio_setReg(uint8_t reg, uint16_t data, I2C_MState* return_state){

    I2C_MState tmp;
    int data_size = 3;
    uint8_t sdata[data_size]; 

    // add register to write
    sdata[0] = reg;
    // convert data to MSB, LSB
    sdata[1]   = data >> 8;
    sdata[2]   = data & 0xFF;
    
    tmp = I2CMASTER_Transmit(RADIO_ADDR,sdata,data_size);
    if (return_state)   *return_state = tmp;

    if ( tmp != TX_data_ACK && tmp != TX_data)
    {
        return Data_Transmit_error;
    }
    return Sucess;
    
}

