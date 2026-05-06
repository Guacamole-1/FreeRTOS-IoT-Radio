/*
 * radio_rtos.h
 *
 * Camada RTOS-safe para o driver Radio.c.
 */

#ifndef RADIO_RTOS_H_
#define RADIO_RTOS_H_

#include <stdint.h>

#include "base.h"
#include "Radio.h"


base_t RADIO_RTOS_INIT(Radio_Data *radio_data);

base_t RADIO_RTOS_SET_VOLUME(Radio_Data *Rdata,
                             I2C_MState *return_state);

base_t RADIO_RTOS_SET_CHANNEL(Radio_Data *Rdata,
                              I2C_MState *return_state);

base_t RADIO_RTOS_SAVE_DATA(Radio_Data *data);

base_t RADIO_RTOS_GET_DATA(Radio_Data *data);

base_t RADIO_RTOS_GET_REG(uint8_t reg,
                          uint16_t *register_data,
                          I2C_MState *return_state);

base_t RADIO_RTOS_SET_REG(uint8_t reg,
                          uint16_t data,
                          I2C_MState *return_state);


#endif /* RADIO_RTOS_H_ */
