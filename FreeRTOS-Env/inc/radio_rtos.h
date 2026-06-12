/**
 * @file radio_rtos.h
 * @brief This header file provides the RTOS-safe API for controlling the radio.
 * @version 1
 * @date 13 May 2026
 * @author Grupo 08
 *
 */

/**
 * @defgroup radio_rtos_module Radio RTOS Module
 * @ingroup free_rtos
 * @brief Thread-safe FreeRTOS wrapper for the radio driver.
 * @{
 */

#ifndef RADIO_RTOS_H_
#define RADIO_RTOS_H_

#include <stdint.h>

#include "base.h"
#include "Radio.h"

/**
 * @brief Initializes the radio subsystem and its RTOS synchronization lock.
 *
 * Initializes the underlying radio driver using the supplied Radio_Data object.
 * If valid saved radio data exists, the lower-level radio driver may update
 * the supplied structure with the stored configuration.
 *
 * Must be called before any other RADIO_RTOS function.
 *
 * @param radio_data Pointer to the radio configuration/state structure.
 *
 * @return SUCCESS on success.
 * @return ALREADY_INITIALIZED if the module was already initialized.
 * @return ERROR or a mapped radio error code on failure.
 */
base_t RADIO_RTOS_INIT(Radio_Data *radio_data);

/**
 * @brief Sets the radio volume in a thread-safe way.
 *
 * Uses the volume value stored in @p Rdata and calls the underlying radio
 * driver while holding the module lock.
 *
 * @param Rdata Pointer to the radio data structure containing the new volume.
 * @param return_state Optional pointer used by the lower-level I2C driver to
 *                     return the I2C operation state.
 *
 * @return SUCCESS on success.
 * @return NOT_INITIALIZED if RADIO_RTOS_INIT was not called.
 * @return ARG_ERROR if @p Rdata is NULL.
 * @return ERROR or a mapped radio error code on failure.
 */
base_t RADIO_RTOS_SET_VOLUME(Radio_Data *Rdata,
                             I2C_MState *return_state);

/**
 * @brief Sets the radio channel/frequency in a thread-safe way.
 *
 * Uses the channel value stored in @p Rdata and calls the underlying radio
 * driver while holding the module lock.
 *
 * @param Rdata Pointer to the radio data structure containing the new channel.
 * @param return_state Optional pointer used by the lower-level I2C driver to
 *                     return the I2C operation state.
 *
 * @return SUCCESS on success.
 * @return NOT_INITIALIZED if RADIO_RTOS_INIT was not called.
 * @return ARG_ERROR if @p Rdata is NULL.
 * @return ERROR or a mapped radio error code on failure.
 */
base_t RADIO_RTOS_SET_CHANNEL(Radio_Data *Rdata,
                              I2C_MState *return_state);

/**
 * @brief Saves the radio data structure to non-volatile memory.
 *
 * The access is protected by the radio RTOS mutex so that no other radio
 * operation can run concurrently while the data is being saved.
 *
 * @param data Pointer to the radio data structure to save.
 *
 * @return SUCCESS on success.
 * @return NOT_INITIALIZED if RADIO_RTOS_INIT was not called.
 * @return ARG_ERROR if @p data is NULL.
 * @return ERROR or a mapped storage error code on failure.
 */
base_t RADIO_RTOS_SAVE_DATA(Radio_Data *data);

/**
 * @brief Reads saved radio data from non-volatile memory.
 *
 * @param data Pointer to the radio data structure that receives the stored data.
 *
 * @return SUCCESS on success.
 * @return NOT_INITIALIZED if RADIO_RTOS_INIT was not called.
 * @return ARG_ERROR if @p data is NULL.
 * @return ERROR or a mapped storage error code on failure.
 */
base_t RADIO_RTOS_GET_DATA(Radio_Data *data);

/**
 * @brief Reads one register from the radio device.
 *
 * @param reg Register address to read.
 * @param register_data Pointer that receives the register value.
 * @param return_state Optional pointer used by the lower-level I2C driver to
 *                     return the I2C operation state.
 *
 * @return SUCCESS on success.
 * @return NOT_INITIALIZED if RADIO_RTOS_INIT was not called.
 * @return ARG_ERROR if @p register_data is NULL.
 * @return ERROR or a mapped radio error code on failure.
 */
base_t RADIO_RTOS_GET_REG(uint8_t reg,
                          uint16_t *register_data,
                          I2C_MState *return_state);

/**
 * @brief Writes one register in the radio device.
 *
 * @param reg Register address to write.
 * @param data Value to write to the selected register.
 * @param return_state Optional pointer used by the lower-level I2C driver to
 *                     return the I2C operation state.
 *
 * @return SUCCESS on success.
 * @return NOT_INITIALIZED if RADIO_RTOS_INIT was not called.
 * @return ERROR or a mapped radio error code on failure.
 */
base_t RADIO_RTOS_SET_REG(uint8_t reg,
                          uint16_t data,
                          I2C_MState *return_state);

#endif /* RADIO_RTOS_H_ */

/** @} */
