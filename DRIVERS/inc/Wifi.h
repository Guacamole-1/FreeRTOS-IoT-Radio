#ifndef WIFI_H_
#define WIFI_H_

/**
 * @file Wifi.h
 * @brief ESP8266 WiFi driver — AT command abstraction for LPC1769 + FreeRTOS.
 *
 * Layered design:
 *   Application
 *       |
 *   WIFI_ConnectAP / WIFI_Send / WIFI_ReceiveUDP  (this layer)
 *       |
 *   WIFI_SendATCommand                            (AT engine)
 *       |
 *   UART_WriteString / UART_GetChar               (HAL)
 *       |
 *   ESP8266 hardware
 */

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Configuration
 * ------------------------------------------------------------------------- */

#define WIFI_DEFAULT_BAUDRATE      115200U   /**< UART baud rate for ESP8266  */
#define WIFI_DEFAULT_TIMEOUT_MS    5000U     /**< General AT command timeout  */
#define WIFI_RX_BUFFER_SIZE        512U      /**< Suggested RX scratch buffer */
/* -------------------------------------------------------------------------
 * Status codes
 * ------------------------------------------------------------------------- */

/**
 * @brief Return codes used by WIFI_SendATCommand().
 */
typedef enum {
    WIFI_OK            = 0,  /**< Command succeeded / expected token found  */
    WIFI_ERROR,              /**< Module replied ERROR or FAIL               */
    WIFI_TIMEOUT,            /**< Timed out waiting for a response           */
    WIFI_INVALID_ARG,        /**< A required pointer argument was NULL       */
    WIFI_SEND_ERROR,         /**< Data transmission failed                   */
    WIFI_NOT_CONNECTED       /**< No active AP or socket connection          */
} WIFI_Status;

/* -------------------------------------------------------------------------
 * Mode
 * ------------------------------------------------------------------------- */

/**
 * @brief ESP8266 operating mode (AT+CWMODE).
 */
typedef enum {
    WIFI_MODE_STATION        = 1,  /**< Station mode (connects to an AP)     */
    WIFI_MODE_SOFTAP         = 2,  /**< SoftAP mode (acts as an AP)          */
    WIFI_MODE_STATION_SOFTAP = 3   /**< Both simultaneously                  */
} WIFI_Mode;

/* -------------------------------------------------------------------------
 * Core AT engine
 * ------------------------------------------------------------------------- */

/**
 * @brief Sends an AT command and waits for a terminal response.
 *
 * Flushes the RX buffer, writes @p cmd to the UART, then collects characters
 * until @p expected is found in the accumulated response, or until "ERROR" /
 * "FAIL" appear, or the timeout expires.
 *
 * Passing @p cmd as an empty string (@c "") skips the write step and only
 * waits for a response — useful when raw data has already been written and
 * you need to wait for "SEND OK".
 *
 * @param cmd          Null-terminated AT command string (e.g. "AT+RST\r\n").
 * @param expected     Token to wait for (e.g. "OK", ">").  May be NULL if
 *                     only error detection is needed.
 * @param response     Optional buffer to store the raw module reply.
 *                     Pass NULL if the reply is not needed.
 * @param response_len Size of @p response, including the null terminator.
 * @param timeout_ms   Maximum time to wait in milliseconds.
 *
 * @return @ref WIFI_OK, @ref WIFI_ERROR, @ref WIFI_TIMEOUT, or
 *         @ref WIFI_INVALID_ARG.
 */
WIFI_Status WIFI_SendATCommand(const char *cmd,
                               const char *expected,
                               char       *response,
                               uint32_t    response_len,
                               uint32_t    timeout_ms);

/* -------------------------------------------------------------------------
 * Initialisation & basic tests
 * ------------------------------------------------------------------------- */

/**
 * @brief Initialises UART and verifies the ESP8266 is responsive.
 *
 * Safe to call multiple times; UART is only initialised once.
 *
 * @return true on success.
 */
bool WIFI_Init(void);

/**
 * @brief Sends "AT" and checks for "OK".
 * @return true if the module responds correctly.
 */
bool WIFI_Test(void);

/**
 * @brief Issues AT+RST and waits for the module to reboot.
 *
 * Clears the AP-connected and socket-open flags.
 *
 * @return true if the module is responsive after reset.
 */
bool WIFI_Reset(void);

/* -------------------------------------------------------------------------
 * Mode control
 * ------------------------------------------------------------------------- */

/**
 * @brief Sets the ESP8266 operating mode (AT+CWMODE).
 * @param mode One of the @ref WIFI_Mode values.
 * @return true on success.
 */
bool WIFI_SetMode(WIFI_Mode mode);

/**
 * @brief Convenience wrapper: sets station mode.
 * @return true on success.
 */
bool WIFI_SetModeStation(void);

/* -------------------------------------------------------------------------
 * Access-point management
 * ------------------------------------------------------------------------- */

/**
 * @brief Lists visible access points (AT+CWLAP).
 * @param response     Buffer for the raw reply.
 * @param response_len Size of @p response.
 * @return true on success.
 */
bool WIFI_ListAPs(char *response, uint32_t response_len);

/**
 * @brief Connects to a WPA/WPA2 access point (AT+CWJAP), with retries.
 *
 * Retries up to three times with a 1 s delay between attempts.
 *
 * @param ssid     Network SSID.
 * @param password Network password.
 * @return true when connected.
 */
bool WIFI_ConnectAP(const char *ssid, const char *password);

/**
 * @brief Disconnects from the current access point (AT+CWQAP).
 * @return true on success.
 */
bool WIFI_DisconnectAP(void);

/**
 * @brief Queries the module for a valid station IP (AT+CIFSR).
 *
 * Updates the internal connected flag.
 *
 * @return true if the module has a valid IP address.
 */
bool WIFI_IsConnected(void);

/**
 * @brief Retrieves the connection status string (AT+CIPSTATUS).
 * @param response     Buffer for the raw reply.
 * @param response_len Size of @p response.
 * @return true on success.
 */
bool WIFI_GetStatus(char *response, uint32_t response_len);

/* -------------------------------------------------------------------------
 * Connection management
 * ------------------------------------------------------------------------- */

/**
 * @brief Opens a TCP connection (AT+CIPSTART="TCP",...).
 * @param ip   Remote IP address or hostname.
 * @param port Remote port.
 * @param timeout in seconds.
 * @return true on success.
 */
bool WIFI_OpenTCP(const char *ip, uint16_t port, const unsigned short timeout_s);

/**
 * @brief Opens a UDP connection (AT+CIPSTART="UDP",...).
 * @param host Remote IP address or hostname.
 * @param port Remote port.
 * @param timeout in seconds.
 * @return true on success.
 */
bool WIFI_OpenUDP(const char *host, uint16_t port, const unsigned short timeout_s);

/**
 * @brief Closes the active TCP/UDP connection (AT+CIPCLOSE).
 * @return true on success.
 */
bool WIFI_Close(void);

/* -------------------------------------------------------------------------
 * Data transfer
 * ------------------------------------------------------------------------- */


/**
 * @brief Sends a binary buffer over the active connection (AT+CIPSEND).
 *
 * Waits for the '>' prompt before writing the payload, then waits for
 * "SEND OK".
 *
 * @param data Pointer to the data to send.
 * @param len  Number of bytes to send.
 * @return Number of bytes sent on success, or -1 on error.
 */
int WIFI_Send(const uint8_t *data, uint32_t len);

/**
 * @brief Receives a UDP datagram by parsing the +IPD response.
 *
 * Implements a small state machine: WAIT_IPD → READ_LEN → READ_DATA.
 * Returns as soon as the expected number of bytes has been received.
 * Discards frames that are larger than @p buffer_len.
 *
 * @param buffer       Destination buffer.
 * @param buffer_len   Capacity of @p buffer.
 * @param timeout_ms   Maximum wait time in milliseconds.
 * @return Number of bytes received on success, or -1 on error/timeout.
 */
int WIFI_ReceiveUDP(uint8_t  *buffer,
                        uint32_t  buffer_len,
                        uint32_t  timeout_ms);

/**
 * @brief Receives data from the active TCP connection.
 *
 * TCP is a stream protocol, so data may arrive split across multiple
 * +IPD frames.  This function accumulates bytes from as many frames as
 * needed until either @p buffer_len bytes have been collected or
 * @p timeout_ms expires.  A partial read (some bytes received before the
 * timeout) is still reported as success so the caller can process
 * whatever arrived.
 *
 * If a single +IPD frame is larger than the remaining buffer space,
 * the excess bytes are silently dropped (the frame header is still
 * consumed so the parser stays synchronised).
 *
 * Typical usage — read an HTTP response into a staging buffer:
 * @code
 *   uint8_t  buf[512];
 *   uint32_t got;
 *   if (WIFI_ReceiveTCP(buf, sizeof(buf), &got, 5000U)) {
 *       // process got bytes in buf
 *   }
 * @endcode
 *
 * @param buffer       Destination buffer.
 * @param buffer_len   Capacity of @p buffer in bytes.
 * @param timeout_ms   Maximum time to wait for data in milliseconds.
 * @return Number of bytes received on success, or -1 on error/timeout.
 */
int WIFI_ReceiveTCP(uint8_t  *buffer,
                        uint32_t  buffer_len,
                        uint32_t  timeout_ms);
#ifdef __cplusplus
}
#endif

#endif /* WIFI_H_ */
