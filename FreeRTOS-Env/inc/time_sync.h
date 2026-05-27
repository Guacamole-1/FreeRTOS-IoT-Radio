/**
 * @file time_sync.h
 * @brief Wi-Fi/NTP time synchronization task.
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
 * This module does NOT save Radio_Data to Flash.
 * Saving is now explicit through the Save option in the menu.
 *
 * @param ssid Wi-Fi SSID. Must remain valid while the task runs.
 * @param password Wi-Fi password. Must remain valid while the task runs.
 * @param priority FreeRTOS task priority.
 * @param stack_depth FreeRTOS stack depth.
 *
 * @return SUCCESS if the task was created or is already running,
 *         ARG_ERROR for invalid arguments,
 *         ERROR if task creation failed.
 */
base_t TIME_SYNC_Start(const char *ssid,
                       const char *password,
                       UBaseType_t priority,
                       uint16_t stack_depth);

bool TIME_SYNC_RequestUnixTime(time_t *unix_time); 
#endif /* TIME_SYNC_H_ */
