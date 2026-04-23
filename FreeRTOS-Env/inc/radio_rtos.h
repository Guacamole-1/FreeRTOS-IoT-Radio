/*
 * radio_rtos.h
 *
 *  Created on: 21/04/2026
 *      Author: rafav
 */

#ifndef RADIO_RTOS_H_
#define RADIO_RTOS_H_


#include "Radio.h"
#include "base.h"



base_t RADIO_RTOS_INIT(Radio_Data* radio_data);
base_t RADIO_RTOS_SET_VOLUME(Radio_Data* Rdata,I2C_MState* return_state);
base_t RADIO_RTOS_SET_CHANNEL(Radio_Data* Rdata, I2C_MState* return_state);


#endif /* RADIO_RTOS_H_ */
