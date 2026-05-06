/*
 * radio_rtos.c
 *
 * Camada RTOS-safe para o driver Radio.c.
 *
 * Nota importante:
 *  - A Flash não precisa de uma camada RTOS separada neste projeto,
 *    porque só é usada através das funções do Radio.c.
 *  - Como todas as funções do Radio.c passam por este mutex,
 *    o acesso à Flash fica protegido indiretamente.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "radio_rtos.h"
#include "Radio.h"
#include "mutex_wrapper.h"

LOCK_DEF;

static base_t radio_state_to_base(Radio_State state) {
    return (state == Sucess) ? SUCCESS : (base_t)(state + 7);
}

static base_t cmd_result_to_base(CMD_RESULTS result) {
    return (result == CMD_SUCCESS) ? SUCCESS : (base_t)(result + 20);
}

base_t RADIO_RTOS_INIT(Radio_Data *radio_data) {
    Radio_State state;

    INIT_NCHECK();
    LOCK_INIT();

    LOCK()
        state = Init_Radio(radio_data);
    UNLOCK()

    return radio_state_to_base(state);
}

base_t RADIO_RTOS_SET_VOLUME(Radio_Data *Rdata, I2C_MState *return_state) {
    Radio_State state;

    INIT_CHECK();

    if (Rdata == NULL) {
        return ARG_ERROR;
    }

    LOCK()
        state = Set_Volume(Rdata, return_state);
    UNLOCK()

    return radio_state_to_base(state);
}

base_t RADIO_RTOS_SET_CHANNEL(Radio_Data *Rdata, I2C_MState *return_state) {
    Radio_State state;

    INIT_CHECK();

    if (Rdata == NULL) {
        return ARG_ERROR;
    }

    LOCK()
        state = Set_Channel(Rdata, return_state);
    UNLOCK()

    return radio_state_to_base(state);
}

base_t RADIO_RTOS_SAVE_DATA(Radio_Data *data) {
    CMD_RESULTS result;

    INIT_CHECK();

    if (data == NULL) {
        return ARG_ERROR;
    }

    LOCK()
        result = Save_RData(data);
    UNLOCK()

    return cmd_result_to_base(result);
}

base_t RADIO_RTOS_GET_DATA(Radio_Data *data) {
    CMD_RESULTS result;

    INIT_CHECK();

    if (data == NULL) {
        return ARG_ERROR;
    }

    LOCK()
        result = Get_RData(data);
    UNLOCK()

    return cmd_result_to_base(result);
}

base_t RADIO_RTOS_GET_REG(uint8_t reg,
                          uint16_t *register_data,
                          I2C_MState *return_state) {
    Radio_State state;

    INIT_CHECK();

    if (register_data == NULL) {
        return ARG_ERROR;
    }

    LOCK()
        state = Radio_getReg(reg, register_data, return_state);
    UNLOCK()

    return radio_state_to_base(state);
}

base_t RADIO_RTOS_SET_REG(uint8_t reg,
                          uint16_t data,
                          I2C_MState *return_state) {
    Radio_State state;

    INIT_CHECK();

    LOCK()
        state = Radio_setReg(reg, data, return_state);
    UNLOCK()

    return radio_state_to_base(state);
}
