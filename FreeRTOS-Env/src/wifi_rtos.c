/**
 * @file wifi_rtos.c
 * @brief Thread-safe FreeRTOS wrapper for the ESP8266 WiFi driver.
 *
 * Every public function acquires the module mutex before calling into
 * the bare-metal Wifi driver, so multiple tasks can share the driver
 * without racing.  The mutex is created once by WIFI_RTOS_INIT and all
 * other functions return NOT_INITIALIZED if it has not been called yet.
 *
 * The receive functions (ReceiveUDP / ReceiveTCP) hold the mutex for the
 * full duration of the receive — this is intentional.  Because the ESP8266
 * has a single UART, no other command may be issued while data is being
 * streamed in.  Callers that need a non-blocking receive should do so from
 * a dedicated task.
 */

#include "wifi_rtos.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "Wifi.h"
#include "mutex_wrapper.h"
/* One mutex shared across all functions in this translation unit */
LOCK_DEF;

/* -------------------------------------------------------------------------
 * Internal helper
 * ------------------------------------------------------------------------- */

static base_t wifi_bool_to_base(bool result)
{
    return result ? SUCCESS : ERROR;
}

/* -------------------------------------------------------------------------
 * Initialisation
 * ------------------------------------------------------------------------- */

base_t WIFI_RTOS_Init(void)
{
    bool result;

    INIT_NCHECK();
    LOCK_INIT();

    LOCK()
        result = WIFI_Init();
    UNLOCK()

    return wifi_bool_to_base(result);
}

/* -------------------------------------------------------------------------
 * Basic module tests
 * ------------------------------------------------------------------------- */

base_t WIFI_RTOS_Test(void)
{
    bool result;

    INIT_CHECK();

    LOCK()
        result = WIFI_Test();
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_Reset(void)
{
    bool result;

    INIT_CHECK();

    LOCK()
        result = WIFI_Reset();
    UNLOCK()

    return wifi_bool_to_base(result);
}

/* -------------------------------------------------------------------------
 * Mode control
 * ------------------------------------------------------------------------- */

base_t WIFI_RTOS_SetModeStation(void)
{
    bool result;

    INIT_CHECK();

    LOCK()
        result = WIFI_SetModeStation();
    UNLOCK()

    return wifi_bool_to_base(result);
}

/* -------------------------------------------------------------------------
 * Access-point management
 * ------------------------------------------------------------------------- */

base_t WIFI_RTOS_ConnectAp(const char *ssid, const char *password)
{
    bool result;

    INIT_CHECK();

    if ((ssid == NULL) || (password == NULL)) {
        return ARG_ERROR;
    }

    LOCK()
        result = WIFI_ConnectAP(ssid, password);
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_DisconnectAp(void)
{
    bool result;

    INIT_CHECK();

    LOCK()
        result = WIFI_DisconnectAP();
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_IsConnected(void)
{
    bool result;

    INIT_CHECK();

    LOCK()
        result = WIFI_IsConnected();
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_GetStatus(char *response, uint32_t response_len)
{
    bool result;

    INIT_CHECK();

    if ((response == NULL) || (response_len == 0U)) {
        return ARG_ERROR;
    }

    LOCK()
        result = WIFI_GetStatus(response, response_len);
    UNLOCK()

    return wifi_bool_to_base(result);
}

/* -------------------------------------------------------------------------
 * Connection management
 * ------------------------------------------------------------------------- */

base_t WIFI_RTOS_OpenTcp(const char *ip, uint16_t port, unsigned short timeout)
{
    bool result;

    INIT_CHECK();

    if ((ip == NULL) || (port == 0U)) {
        return ARG_ERROR;
    }

    LOCK()
        result = WIFI_OpenTCP(ip, port,timeout);
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_OpenUdp(const char *host, uint16_t port, unsigned short timeout)
{
    bool result;

    INIT_CHECK();

    if ((host == NULL) || (port == 0U)) {
        return ARG_ERROR;
    }

    LOCK()
        result = WIFI_OpenUDP(host, port,timeout);
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_Close(void)
{
    bool result;

    INIT_CHECK();

    LOCK()
        result = WIFI_Close();
    UNLOCK()

    return wifi_bool_to_base(result);
}

/* -------------------------------------------------------------------------
 * Data transfer
 * ------------------------------------------------------------------------- */

int WIFI_RTOS_Send(const uint8_t *data, uint32_t len)
{
    int sent;

    INIT_CHECK();

    if ((data == NULL) || (len == 0U)) {
        return ARG_ERROR;
    }

    LOCK()
        sent = WIFI_Send(data, len);
    UNLOCK()

    return sent;
}

int WIFI_RTOS_ReceiveUdp(uint8_t  *buffer,
                             uint32_t  buffer_len,
                             uint32_t  timeout_ms)
{
    int received;

    INIT_CHECK();

    if ((buffer == NULL) || (buffer_len == 0U)) {
        return ARG_ERROR;
    }

    LOCK()
        received = WIFI_ReceiveUDP(buffer, buffer_len, timeout_ms);
    UNLOCK()

    return received;
}

int WIFI_RTOS_ReceiveTcp(uint8_t  *buffer,
                             uint32_t  buffer_len,
                             uint32_t  timeout_ms)
{
    int received;

    INIT_CHECK();

    if ((buffer == NULL) || (buffer_len == 0U)) {
        return ARG_ERROR;
    }

    LOCK()
        received = WIFI_ReceiveTCP(buffer, buffer_len, timeout_ms);
    UNLOCK()

    return received;
}
