/**
 * @file wifi_rtos.h
 * @brief Thread-safe FreeRTOS wrapper for the ESP8266 WiFi driver.
 *
 * All functions are safe to call from multiple FreeRTOS tasks.
 * A single mutex serialises access to the underlying UART/AT layer.
 *
 * Call order:
 * @code
 *   WIFI_RTOS_Init();
 *   WIFI_RTOS_SetModeStation();
 *   WIFI_RTOS_ConnectAp("ssid", "pass");
 *
 *   // TCP example
 *   WIFI_RTOS_OpenTcp("192.168.1.1", 80);
 *   WIFI_RTOS_Send(data, len);
 *   WIFI_RTOS_ReceiveTcp(buf, sizeof(buf), &got, 5000U);
 *   WIFI_RTOS_Close();
 * @endcode
 */

#ifndef WIFI_RTOS_H_
#define WIFI_RTOS_H_

#include <stdint.h>
#include <stdbool.h>

#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
  Initialisation
  ------------------------------------------------------------------------- */

/**
 * @brief Initialises the WiFi driver and creates the module mutex.
 *
 * Must be called once before any other WIFI_RTOS function.
 * Subsequent calls return ALREADY_INITIALIZED without re-initialising.
 *
 * @return SUCCESS, ALREADY_INITIALIZED, or ERROR.
 */
base_t WIFI_RTOS_Init(void);

/* -------------------------------------------------------------------------
  Basic module tests
  ------------------------------------------------------------------------- */

/**
 * @brief Sends AT and checks for OK.
 * @return SUCCESS, ERROR, NOT_INITIALIZED, or LOCK_ERROR.
 */
base_t WIFI_RTOS_Test(void);

/**
 * @brief Issues AT+RST and waits for the module to reboot.
 *
 * Clears the internal AP-connected and socket-open flags.
 *
 * @return SUCCESS, ERROR, NOT_INITIALIZED, or LOCK_ERROR.
 */
base_t WIFI_RTOS_Reset(void);

/* -------------------------------------------------------------------------
  Mode control
  ------------------------------------------------------------------------- */

/**
 * @brief Sets the ESP8266 to station mode (AT+CWMODE=1).
 * @return SUCCESS, ERROR, NOT_INITIALIZED, or LOCK_ERROR.
 */
base_t WIFI_RTOS_SetModeStation(void);

/* -------------------------------------------------------------------------
  Access-point management
  ------------------------------------------------------------------------- */

/**
 * @brief Connects to a WPA/WPA2 access point (AT+CWJAP), with retries.
 *
 * @param ssid     Network SSID. Must remain valid for the duration of the call.
 * @param password Network password. Must remain valid for the duration of the call.
 * @return SUCCESS, ERROR, ARG_ERROR, NOT_INITIALIZED, or LOCK_ERROR.
 */
base_t WIFI_RTOS_ConnectAp(const char *ssid, const char *password);

/**
 * @brief Disconnects from the current access point (AT+CWQAP).
 * @return SUCCESS, ERROR, NOT_INITIALIZED, or LOCK_ERROR.
 */
base_t WIFI_RTOS_DisconnectAp(void);

/**
 * @brief Checks whether the module has a valid station IP (AT+CIFSR).
 * @return SUCCESS if connected, ERROR if not, NOT_INITIALIZED, or LOCK_ERROR.
 */
base_t WIFI_RTOS_IsConnected(void);

/**
 * @brief Retrieves the raw AT+CIPSTATUS response string.
 *
 * @param response     Caller-supplied buffer for the reply.
 * @param response_len Size of @p response in bytes.
 * @return SUCCESS, ERROR, ARG_ERROR, NOT_INITIALIZED, or LOCK_ERROR.
 */
base_t WIFI_RTOS_GetStatus(char *response, uint32_t response_len);

/* -------------------------------------------------------------------------
  Connection management
  ------------------------------------------------------------------------- */

/**
 * @brief Opens a TCP connection (AT+CIPSTART="TCP",...).
 *
 * @param ip   Remote IP address or hostname string.
 * @param port Remote port (must be non-zero).
 * @param timeout in seconds.
 * @return SUCCESS, ERROR, ARG_ERROR, NOT_INITIALIZED, or LOCK_ERROR.
 */
base_t WIFI_RTOS_OpenTcp(const char *ip, uint16_t port, unsigned short timeout);

/**
 * @brief Opens a UDP connection (AT+CIPSTART="UDP",...).
 *
 * @param host Remote IP address or hostname string.
 * @param port Remote port (must be non-zero).
 * @param timeout in seconds.
 * @return SUCCESS, ERROR, ARG_ERROR, NOT_INITIALIZED, or LOCK_ERROR.
 */
base_t WIFI_RTOS_OpenUdp(const char *host, uint16_t port, unsigned short timeout);

/**
 * @brief Closes the active TCP/UDP connection (AT+CIPCLOSE).
 * @return SUCCESS, ERROR, NOT_INITIALIZED, or LOCK_ERROR.
 */
base_t WIFI_RTOS_Close(void);

/* -------------------------------------------------------------------------
  Data transfer
  ------------------------------------------------------------------------- */

/**
 * @brief Sends a binary buffer over the active connection (AT+CIPSEND).
 *
 * @param data Pointer to the data buffer.
 * @param len  Number of bytes to send (must be non-zero).
 * @return Number of bytes sent on success, or -1 for error.
 */
int WIFI_RTOS_Send(const uint8_t *data, uint32_t len);

/**
 * @brief Receives one UDP datagram from the active connection.
 *
 * Waits for a single +IPD frame that fits entirely within @p buffer.
 * Frames larger than @p buffer_len are discarded and the function keeps
 * waiting until a frame that fits arrives or the timeout expires.
 *
 * The mutex is held for the full duration of the receive, so no other
 * WiFi call can interrupt it.  Use a dedicated task for long receives.
 *
 * @param buffer       Destination buffer.
 * @param buffer_len   Capacity of @p buffer in bytes.
 * @param received_len Set to the number of bytes received on SUCCESS.
 * @param timeout_ms   Maximum time to wait in milliseconds.
 * @return Number of bytes received on success, or -1 for timeout/failure.
 */
int WIFI_RTOS_ReceiveUdp(uint8_t  *buffer,
                             uint32_t  buffer_len,
                             uint32_t  timeout_ms);

/**
 * @brief Receives a TCP stream into @p buffer, accumulating across frames.
 *
 * Keeps reading +IPD frames until @p buffer_len bytes have been collected
 * or @p timeout_ms expires.  A partial read (some bytes received before the
 * timeout) is still reported as SUCCESS so the caller can process whatever
 * arrived.
 *
 * The mutex is held for the full duration of the receive, so no other
 * WiFi call can interrupt it.  Use a dedicated task for long receives.
 *
 * @param buffer       Destination buffer.
 * @param buffer_len   Capacity of @p buffer in bytes.
 * @param received_len Set to the total bytes stored on SUCCESS.
 * @param timeout_ms   Maximum time to wait in milliseconds.
 * @return Number of bytes received on success, or -1 for timeout/failure.
 */
int WIFI_RTOS_ReceiveTcp(uint8_t  *buffer,
                          uint32_t  buffer_len,
                          uint32_t  timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_RTOS_H_ */
