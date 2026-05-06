/**
 * @file time_sync.c
 * @brief Wi-Fi/NTP time synchronization task.
 */

#include "time_sync.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"

#include "base.h"
#include "clock.h"
#include "display.h"
#include "Wifi.h"

#define NTP_SERVER                 "pool.ntp.org"
#define NTP_PORT                   123U
#define NTP_PACKET_SIZE            48U
#define NTP_RESPONSE_SIZE          64U
#define NTP_TIMEOUT_MS             10000U
#define NTP_TO_UNIX_OFFSET         2208988800UL

/*
 * NTP returns UTC.
 * Portugal continental:
 * - winter: UTC+0
 * - summer: UTC+1
 *
 * For now, as requested, use UTC+1.
 */
#define TIMEZONE_OFFSET_SECONDS    3600UL

typedef struct {
    const char *ssid;
    const char *password;
} TIME_SYNC_Context;

static TIME_SYNC_Context time_sync_context;
static TaskHandle_t time_sync_task_handle = NULL;

static void TIME_SYNC_Display(const char *line1, const char *line2) {
    DISPLAY_Send((DISPLAY_Item){ CLEAR, NULL });
    DISPLAY_Printf("%-16.16s%-16.16s", line1, line2);
}

static bool TIME_SYNC_RequestUnixTime(time_t *unix_time) {
    uint8_t ntp_packet[NTP_PACKET_SIZE];
    uint8_t ntp_response[NTP_RESPONSE_SIZE];
    uint32_t received_len = 0U;
    uint32_t ntp_seconds;
    uint32_t unix_seconds;

    if (unix_time == NULL) {
        return false;
    }

    memset(ntp_packet, 0, sizeof(ntp_packet));
    memset(ntp_response, 0, sizeof(ntp_response));

    /*
     * NTP request first byte:
     * LI, Version, Mode.
     */
    ntp_packet[0] = 0xE3U;

    if (!WIFI_OpenUDP(NTP_SERVER, NTP_PORT)) {
        return false;
    }

    if (!WIFI_Send(ntp_packet, NTP_PACKET_SIZE)) {
        (void)WIFI_Close();
        return false;
    }

    if (!WIFI_ReceiveUDP(ntp_response,
                         sizeof(ntp_response),
                         &received_len,
                         NTP_TIMEOUT_MS)) {
        (void)WIFI_Close();
        return false;
    }

    (void)WIFI_Close();

    if (received_len < NTP_PACKET_SIZE) {
        return false;
    }

    /*
     * Transmit Timestamp seconds starts at byte 40.
     * Network packets are big-endian.
     */
    ntp_seconds =
        ((uint32_t)ntp_response[40] << 24) |
        ((uint32_t)ntp_response[41] << 16) |
        ((uint32_t)ntp_response[42] << 8)  |
        ((uint32_t)ntp_response[43]);

    unix_seconds = ntp_seconds - NTP_TO_UNIX_OFFSET;
    unix_seconds += TIMEZONE_OFFSET_SECONDS;

    *unix_time = (time_t)unix_seconds;

    return true;
}

static void TIME_SYNC_Task(void *pvParameters) {
    TIME_SYNC_Context *ctx = (TIME_SYNC_Context *)pvParameters;
    time_t unix_time;

    if ((ctx == NULL) || (ctx->ssid == NULL) || (ctx->password == NULL)) {
        time_sync_task_handle = NULL;
        vTaskDelete(NULL);
    }

    TIME_SYNC_Display("Time Sync", "WiFi init...");
    vTaskDelay(pdMS_TO_TICKS(500));

    if (!WIFI_Init()) {
        TIME_SYNC_Display("Time Sync", "WiFi init fail");
        time_sync_task_handle = NULL;
        vTaskDelete(NULL);
    }

    TIME_SYNC_Display("Time Sync", "Station...");
    vTaskDelay(pdMS_TO_TICKS(500));

    if (!WIFI_SetModeStation()) {
        TIME_SYNC_Display("Time Sync", "Mode fail");
        time_sync_task_handle = NULL;
        vTaskDelete(NULL);
    }

    TIME_SYNC_Display("Time Sync", "Connecting...");
    vTaskDelay(pdMS_TO_TICKS(500));

    if (!WIFI_ConnectAP(ctx->ssid, ctx->password)) {
        TIME_SYNC_Display("Time Sync", "WiFi fail");
        time_sync_task_handle = NULL;
        vTaskDelete(NULL);
    }

    TIME_SYNC_Display("Time Sync", "NTP...");
    vTaskDelay(pdMS_TO_TICKS(500));

    if (!TIME_SYNC_RequestUnixTime(&unix_time)) {
        TIME_SYNC_Display("Time Sync", "NTP fail");
        time_sync_task_handle = NULL;
        vTaskDelete(NULL);
    }

    TIME_SYNC_Display("Time Sync", "Set clock...");
    vTaskDelay(pdMS_TO_TICKS(500));

    if (CLOCK_SetSeconds(unix_time) != SUCCESS) {
        TIME_SYNC_Display("Time Sync", "Clock fail");
        time_sync_task_handle = NULL;
        vTaskDelete(NULL);
    }

    /*
     * Important:
     * This module only updates CLOCK/RTC.
     * It does NOT save to Flash. Save is explicit in the menu.
     */
    TIME_SYNC_Display("Time Sync", "OK");
    vTaskDelay(pdMS_TO_TICKS(2000));

    time_sync_task_handle = NULL;
    vTaskDelete(NULL);
}

base_t TIME_SYNC_Start(const char *ssid,
                       const char *password,
                       UBaseType_t priority,
                       uint16_t stack_depth) {
    if ((ssid == NULL) || (password == NULL)) {
        return ARG_ERROR;
    }

    if (time_sync_task_handle != NULL) {
        return SUCCESS;
    }

    time_sync_context.ssid = ssid;
    time_sync_context.password = password;

    if (xTaskCreate(TIME_SYNC_Task,
                    "TimeSync",
                    stack_depth,
                    &time_sync_context,
                    priority,
                    &time_sync_task_handle) != pdPASS) {
        time_sync_task_handle = NULL;
        return ERROR;
    }

    return SUCCESS;
}
