/**
 * @file publisher.h
 * @brief This header file provides the API for starting the MQTT publisher task.
 * @version 1
 * @date 12 Jun 2026
 * @author Grupo 08
 *
 */

/**
 * @defgroup publisher_module MQTT Publisher Module
 * @ingroup free_rtos
 * @brief FreeRTOS task responsible for MQTT communication.
 * @{
 */

#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "base.h"

/**
 * @brief Starts the MQTT publisher task.
 *
 * Creates a FreeRTOS task that connects to the MQTT broker, sends telemetry
 * and listens for subscribed MQTT messages.
 *
 * @param pvParameters Pointer passed to the created task. In this project it
 *                     is expected to point to the current Radio_Data object.
 *
 * @return SUCCESS if the task was created successfully.
 * @return ERROR if task creation failed.
 */
base_t Start_Publisher_Task(void *pvParameters);

#endif /* PUBLISHER_H */

/** @} */
