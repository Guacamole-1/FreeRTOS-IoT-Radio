/**
 * @file time_sync.h
 * @brief This header file provides the API for Wi-Fi/NTP time synchronization.
 * @version 1
 * @date 24 May 2026
 * @author Grupo 08
 *
 */

/**
 * @defgroup time_sync_module Time Sync Module
 * @ingroup free_rtos
 * @brief NTP-based time synchronization helper for the RTC/clock module.
 * @{
 */

#ifndef TIME_SYNC_H_
#define TIME_SYNC_H_

#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

#include "base.h"

/**
 * @brief Starts a task that connects to Wi-Fi, requests NTP time and updates CLOCK/RTC.
 *
 * This module does not save Radio_Data to Flash. Saving is explicit through
 * the Save option in the menu.
 *
 * @param ssid Wi-Fi SSID. Must remain valid while the task runs.
 * @param password Wi-Fi password. Must remain valid while the task runs.
 * @param priority FreeRTOS task priority.
 * @param stack_depth FreeRTOS stack depth.
 *
 * @return SUCCESS if the task was created or is already running.
 * @return ARG_ERROR for invalid arguments.
 * @return ERROR if task creation failed.
 */
base_t TIME_SYNC_Start(const char *ssid,
                       const char *password,
                       UBaseType_t priority,
                       uint16_t stack_depth);

/**
 * @brief Requests the current Unix time from an NTP server.
 *
 * Performs the NTP request synchronously and stores the received Unix time in
 * @p unix_time. The Wi-Fi connection must be available before calling this
 * function.
 *
 * @param unix_time Pointer to the variable that receives the Unix timestamp.
 *
 * @return true if a valid NTP timestamp was received.
 * @return false if @p unix_time is NULL, the request failed, or no valid reply
 *         was received.
 */
bool TIME_SYNC_RequestUnixTime(time_t *unix_time);

#endif /* TIME_SYNC_H_ */

/** @} */
