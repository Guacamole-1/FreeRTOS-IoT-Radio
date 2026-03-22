/**
* @file Radio.h
* @brief This header file provides the API for controlling the Radio
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
* @defgroup Radio Radio functions
* @brief This module provides core functions for Radio
* @{
*/

#ifndef RADIO_H_
#define RADIO_H_

#include "I2C.h"
#include "RTC.h"
#include "Flash.h"

//*--- REGISTER 2---
#define DHIZ            (1 << 15)  // 0=Hi-Z, 1=normal 
#define DMUTE           (1 << 14)  // 0=mute, 1=audio on 
#define MONO            (1 << 13)  // 0=stereo, 1=force mono 
#define BASS            (1 << 12)  // 0=off, 1=bass boost 
#define RCLK_NCAL_MODE  (1 << 11)  // 0=RCLK always, 1=non-calibrate mode 
#define RCLK_DIRECT     (1 << 10)  // 1=RCLK direct input mode 
#define SEEKUP          (1 << 9)   // 0=seek down, 1=seek up 
#define SEEK            (1 << 8)   // 1=start seek 
#define SKMODE          (1 << 7)   // 0=wrap band, 1=stop at band limit 
#define CLK_MODE        (1 << 4)   // 3 bits 
#define RDS_EN          (1 << 3)   // enable RDS/RBDS 
#define NEW_METHOD      (1 << 2)   // new demod method (better sensitivity) 
#define SOFT_RESET      (1 << 1)   // write 1 to reset 
#define POWER_UP        (1 << 0)   // power-up 

//*--- REGISTER 3---

// Channel Spacing.
// 00 = 100 kHz
// 01 = 200 kHz
// 10 = 50kHz
// 11 = 25KHz
#define SPACE(x)        (x & 0b11)   
#define BAND            (00 << 2)           // band 00 Europe 
#define TUNE            (1 << 4)            // Tune enable
#define CHAN(x)         ((x & 0x3FF) << 6)  // Frequency = Channel Spacing (kHz) x CHAN + 87.0 MHz (10 bits)

//*--- REGISTER 4---

#define RBDS            (1 << 13)
#define RDS_FIFO_CLR    (1 << 10)
#define RDS_FIFO_EN     (1 << 12)

//*--- REGISTER 5---

#define INT_MODE        (1 << 15)
#define SEEK_TH         (0b1000 << 8)        // Seek SNR threshold value 
#define LNA_PORT        (0b10 << 6)                   
#define VOLUME(x)       (x & 0xF)            // DAC Gain Control Bits (Volume). 0000=min; 1111=max 

//*--- REGISTER 10---

#define READ_CHAN       (0x3FF)              // Read Channel. Frequency = Channel Spacing (kHz) x READCHAN[9:0]+ 87.0 MHz 
#define SF              (1 << 13)            // Seek Fail. 0 = Seek successful; 1 = Seek failure
#define STC             (1 << 14)            // Seek/Tune Complete. 0 = Not complete 1 = Complete 

#define RADIO_ADDR              0x11 
#define SPACING                 100000.0
#define CURRENT_FREQ(chan)      ((SPACING*chan)/1000000.0 + 87.0)       //gets freq in MHz
#define STARTING_VOLUME         0b1011
#define RADIO_DATA_ID           "Vol and Channel DataG06"

typedef enum {
    Sucess, Data_Transmit_error, Data_Receive_error, Reg_select_error, Flash_error
}Radio_State;

typedef struct
{
    char id[24];
    uint8_t volume;
    uint16_t channel;
    tm time;
}Radio_Data;

/**
 * @brief Initializes Radio (along with DELAY and I2C) 
 *  if there is saved data on flash that will be loaded onto the radio and not radio_data
 * 
 * @param radio_data radio data to initialize radio
 * @return Radio_State return code
 */
Radio_State Init_Radio(Radio_Data* radio_data);
/**
 * @brief Sets the volume stored on Rdata
 * 
 * @param Rdata data struct with volume
 * @param return_state optional argument to store I2C return state
 * @return Radio_State return code
 */
Radio_State Set_Volume(Radio_Data* Rdata, I2C_MState* return_state);
/**
 * @brief saves data to Flash memory
 * @return CMD_RESULTS return code
 */
CMD_RESULTS Save_RData(Radio_Data* data);

/**
 * @brief Gets Radio_data from Flash memory, if present
 * 
 * @return CMD_RESULTS return code
 */
CMD_RESULTS Get_RData(Radio_Data* data);

/**
 * @brief Sets the channel stored on Rdata
 * 
 * @param Rdata data struct with channel
 * @param return_state optional argument to store I2C return state
 * @return Radio_State return code
 */
Radio_State Set_Channel(Radio_Data* Rdata, I2C_MState* return_state);
/**
 * @brief Gets radio register 
 * 
 * @param reg register to fetch
 * @param register_data data returned by the radio
 * @param return_state optional argument to store I2C return state
 * @return Radio_State return code
 */
Radio_State Radio_getReg(uint8_t reg, uint16_t* register_data, I2C_MState* return_state);

/**
 * @brief Sets radio register 
 * 
 * @param reg register to set
 * @param data data to send to register
 * @param return_state optional argument to store I2C return state
 * @return Radio_State 
 */
Radio_State Radio_setReg(uint8_t reg, uint16_t data, I2C_MState* return_state);
#endif /* RADIO_H_ */

/**
* @}
*/

/**
* @}
*/