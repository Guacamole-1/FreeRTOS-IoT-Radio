/*
 * radio_rtos.c
 *
 *  Created on: 21/04/2026
 *      Author: rafav
 */


#include "FreeRTOS.h"

/* Kernel includes. */
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "radio_rtos.h"
#include "Radio.h"
#include "mutex_wrapper.h"


LOCK_DEF;

base_t RADIO_RTOS_INIT(Radio_Data* radio_data){
	INIT_NCHECK
	LOCK_INIT
	Init_Radio(radio_data);

	return SUCCESS;
}

base_t RADIO_RTOS_SET_VOLUME(Radio_Data* Rdata,I2C_MState* return_state){
	base_t state;
	LOCK
	configASSERT(Rdata != NULL);
	state = Set_Volume(Rdata,return_state);
	UNLOCK
	return state == 0 ? state : state + 7;
}

base_t RADIO_RTOS_SET_CHANNEL(Radio_Data* Rdata, I2C_MState* return_state){
	base_t state;
	LOCK
	configASSERT(Rdata != NULL);
	state = Set_Channel(Rdata,return_state);
	UNLOCK
	return state == 0 ? state : state + 7;
}






