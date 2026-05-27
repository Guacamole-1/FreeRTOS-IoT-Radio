#include "Wifi.h"

#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "Delay.h"

/* -------------------------------------------------------------------------
 * Internal constants
 * ------------------------------------------------------------------------- */

#define WIFI_CMD_BUFFER_SIZE    128U
#define WIFI_CONNECT_RETRIES    3U
#define WIFI_RESET_BOOT_MS      3000U
#define WIFI_INIT_SETTLE_MS     500U

/* -------------------------------------------------------------------------
 * Module state
 * ------------------------------------------------------------------------- */

static bool s_initialized  = false;
static bool s_connected_ap = false;
static bool s_conn_open    = false;   /* TCP/UDP socket open */

/* -------------------------------------------------------------------------
 * Private helpers
 * ------------------------------------------------------------------------- */

/**
 * @brief Appends one character into a null-terminated buffer, keeping it safe.
 */
static void wifi_store_char(char *buf, uint32_t buf_len,
                            uint32_t *idx, char c)
{
    if ((buf != NULL) && (buf_len > 1U) && (*idx < (buf_len - 1U))) {
        buf[(*idx)++] = c;
        buf[*idx]     = '\0';
    }
}

/**
 * @brief Returns true if @p response contains @p token.
 */
static bool wifi_contains(const char *response, const char *token)
{
    return (response != NULL) && (token != NULL) &&
           (strstr(response, token) != NULL);
}

/**
 * @brief KMP-style single-character sequence matcher.
 *
 * Feed one received character at a time.  Returns true (and resets *matched)
 * when the full sequence has been matched.
 *
 * @param[in]  c        Received character.
 * @param[in]  seq      Null-terminated sequence to match.
 * @param[out] matched  Running match counter (must be zero-initialised by caller).
 * @return true when the sequence is fully matched.
 */
static bool wifi_match_seq(char c, const char *seq, uint32_t *matched)
{
    if (c == seq[*matched]) {
        (*matched)++;
        if (seq[*matched] == '\0') {
            *matched = 0U;
            return true;
        }
    } else {
        /* Restart: check if this character starts a new match */
        *matched = (c == seq[0]) ? 1U : 0U;
    }
    return false;
}

/* -------------------------------------------------------------------------
 * Core AT engine
 * ------------------------------------------------------------------------- */

WIFI_Status WIFI_SendATCommand(const char *cmd,
                               const char *expected,
                               char       *response,
                               uint32_t    response_len,
                               uint32_t    timeout_ms)
{
    char     internal_buf[WIFI_CMD_BUFFER_SIZE];
    char    *active_buf;
    uint32_t active_len;
    uint32_t idx   = 0U;
    uint32_t start;
    unsigned char ch;

    if (cmd == NULL) {
        return WIFI_INVALID_ARG;
    }

    /* Use caller's buffer when provided; fall back to local one otherwise.
     * Either way we always have a buffer to search for terminal tokens. */
    if ((response != NULL) && (response_len > 0U)) {
        active_buf = response;
        active_len = response_len;
    } else {
        active_buf = internal_buf;
        active_len = sizeof(internal_buf);
    }
    active_buf[0] = '\0';

    UART_FlushRx();

    /* Empty command is legal: used to wait for a deferred response
     * (e.g. "SEND OK" after raw data has already been written). */
    if (cmd[0] != '\0') {
        UART_WriteString(cmd);
    }

    start = Get_Tick();

    while (DELAY_GetElapsedMillis(start) < timeout_ms) {
        while (UART_GetChar(&ch)) {
            wifi_store_char(active_buf, active_len, &idx, (char)ch);

            /* Positive terminal */
            if ((expected != NULL) && wifi_contains(active_buf, expected)) {
                return WIFI_OK;
            }

            /* Negative terminals — always checked */
            if (wifi_contains(active_buf, "ERROR") ||
                wifi_contains(active_buf, "FAIL")) {
                return WIFI_ERROR;
            }
        }
    }

    return WIFI_TIMEOUT;
}

/* -------------------------------------------------------------------------
 * Initialisation & basic tests
 * ------------------------------------------------------------------------- */

bool WIFI_Init(void)
{
    if (!s_initialized) {
        if (!UART_Initialize(WIFI_DEFAULT_BAUDRATE)) {
            return false;
        }
        DELAY_Milliseconds(WIFI_INIT_SETTLE_MS);
        UART_FlushRx();
        s_initialized  = true;
        s_connected_ap = false;
        s_conn_open    = false;
    }

    /* Try the module up to two times before giving up */
    if (WIFI_Test()) {
        return true;
    }
    DELAY_Milliseconds(WIFI_INIT_SETTLE_MS);
    UART_FlushRx();
    return WIFI_Test();
}

bool WIFI_Test(void)
{
    return WIFI_SendATCommand("AT\r\n", "OK",
                              NULL, 0U, 1000U) == WIFI_OK;
}

bool WIFI_Reset(void)
{
    WIFI_Status st;

    st = WIFI_SendATCommand("AT+RST\r\n", "OK",
                            NULL, 0U, 2000U);
    DELAY_Milliseconds(WIFI_RESET_BOOT_MS);   /* wait for boot banner */
    UART_FlushRx();

    s_connected_ap = false;
    s_conn_open    = false;

    return (st == WIFI_OK) || WIFI_Test();
}

/* -------------------------------------------------------------------------
 * Mode control
 * ------------------------------------------------------------------------- */

bool WIFI_SetMode(WIFI_Mode mode)
{
    char cmd[WIFI_CMD_BUFFER_SIZE];

    if ((mode < WIFI_MODE_STATION) || (mode > WIFI_MODE_STATION_SOFTAP)) {
        return false;
    }
    snprintf(cmd, sizeof(cmd), "AT+CWMODE=%d\r\n", (int)mode);
    return WIFI_SendATCommand(cmd, "OK",
                              NULL, 0U,
                              WIFI_DEFAULT_TIMEOUT_MS) == WIFI_OK;
}

bool WIFI_SetModeStation(void)
{
    return WIFI_SetMode(WIFI_MODE_STATION);
}

/* -------------------------------------------------------------------------
 * Access-point management
 * ------------------------------------------------------------------------- */

bool WIFI_ListAPs(char *response, uint32_t response_len)
{
    return WIFI_SendATCommand("AT+CWLAP\r\n", "OK",
                              response, response_len,
                              10000U) == WIFI_OK;
}

bool WIFI_ConnectAP(const char *ssid, const char *password)
{
    char     cmd[WIFI_CMD_BUFFER_SIZE];
    uint32_t attempt;

    if ((ssid == NULL) || (password == NULL)) {
        return false;
    }

    snprintf(cmd, sizeof(cmd),
             "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);

    for (attempt = 0U; attempt < WIFI_CONNECT_RETRIES; attempt++) {
        if (WIFI_SendATCommand(cmd, "OK",
                               NULL, 0U, 20000U) == WIFI_OK) {
            s_connected_ap = true;
            return true;
        }
        DELAY_Milliseconds(1000U);
    }

    return false;
}

bool WIFI_DisconnectAP(void)
{
    bool ok = WIFI_SendATCommand("AT+CWQAP\r\n", "OK",
                                 NULL, 0U,
                                 WIFI_DEFAULT_TIMEOUT_MS) == WIFI_OK;
    if (ok) {
        s_connected_ap = false;
    }
    return ok;
}

bool WIFI_IsConnected(void)
{
    char     response[WIFI_CMD_BUFFER_SIZE];
    WIFI_Status st;

    st = WIFI_SendATCommand("AT+CIFSR\r\n", "STAIP",
                            response, sizeof(response),
                            WIFI_DEFAULT_TIMEOUT_MS);

    s_connected_ap = (st == WIFI_OK);
    return s_connected_ap;
}

bool WIFI_GetStatus(char *response, uint32_t response_len)
{
    return WIFI_SendATCommand("AT+CIPSTATUS\r\n", "OK",
                              response, response_len,
                              WIFI_DEFAULT_TIMEOUT_MS) == WIFI_OK;
}

/* -------------------------------------------------------------------------
 * Connection (TCP / UDP)
 * ------------------------------------------------------------------------- */

/**
 * @brief Common helper: sets single-connection mode and opens a CIPSTART.
 */
static bool wifi_open_conn(const char *type,
                           const char *host,
                           uint16_t    port,
						   const unsigned short timeout_s)
{
    char cmd[WIFI_CMD_BUFFER_SIZE];

    if (host == NULL) {
        return false;
    }

    /* Force single-connection mode */
    if (WIFI_SendATCommand("AT+CIPMUX=0\r\n", "OK",
                           NULL, 0U,
                           WIFI_DEFAULT_TIMEOUT_MS) != WIFI_OK) {
        return false;
    }

    snprintf(cmd, sizeof(cmd),
             "AT+CIPSTART=\"%s\",\"%s\",%u,%u\r\n",
             type, host, (unsigned int)port,timeout_s);

    if (WIFI_SendATCommand(cmd, "OK",
                           NULL, 0U, 10000U) == WIFI_OK) {
        s_conn_open = true;
        return true;
    }
    return false;
}

bool WIFI_OpenTCP(const char *ip, uint16_t port, const unsigned short timeout_s)
{
    return wifi_open_conn("TCP", ip, port, timeout_s);
}

bool WIFI_OpenUDP(const char *host, uint16_t port, const unsigned short timeout_s)
{
    return wifi_open_conn("UDP", host, port, timeout_s);
}

bool WIFI_Close(void)
{
    bool ok = WIFI_SendATCommand("AT+CIPCLOSE\r\n", "OK",
                                 NULL, 0U,
                                 WIFI_DEFAULT_TIMEOUT_MS) == WIFI_OK;
    if (ok) {
        s_conn_open = false;
    }
    return ok;
}

/* -------------------------------------------------------------------------
 * Data transfer
 * ------------------------------------------------------------------------- */

int WIFI_Send(const uint8_t *data, uint32_t len)
{
    char cmd[WIFI_CMD_BUFFER_SIZE];

    if ((data == NULL) || (len == 0U)) {
        return -1;
    }

    snprintf(cmd, sizeof(cmd),
             "AT+CIPSEND=%lu\r\n", (unsigned long)len);

    /* Wait for the '>' prompt before pushing raw bytes */
    if (WIFI_SendATCommand(cmd, ">",
                           NULL, 0U,
                           WIFI_DEFAULT_TIMEOUT_MS) != WIFI_OK) {
        return -1;
    }

    UART_WriteBuffer(data, len);

    /* Wait for the deferred "SEND OK" confirmation */
    if (WIFI_SendATCommand("", "SEND OK",
                           NULL, 0U, 10000U) != WIFI_OK) {
        return -1;
    }

    return (int32_t)len;
}

/* -------------------------------------------------------------------------
 * Shared +IPD receive engine
 * ------------------------------------------------------------------------- */

/**
 * @brief Behaviour flags passed to wifi_receive_ipd().
 */
typedef enum {
    /**
     * UDP mode: treat the buffer as a single datagram slot.
     * A frame larger than buffer_len is discarded and the function keeps
     * waiting until a frame that fits arrives.
     */
    WIFI_IPD_DISCARD_OVERSIZE = 0U,

    /**
     * TCP mode: accumulate across multiple +IPD frames until the caller's
     * buffer is full or the timeout expires.  Frames that would overflow
     * the remaining space are truncated (bytes beyond the buffer edge are
     * dropped) rather than discarded.
     */
    WIFI_IPD_ACCUMULATE       = 1U
} WIFI_IPD_Mode;

/**
 * @brief Core +IPD parser shared by ReceiveUDP and ReceiveTCP.
 *
 * The ESP8266 delivers incoming data as:
 * @code
 *   +IPD,<len>:<data...>
 * @endcode
 * This function runs a state machine over the raw UART byte stream to find
 * that header and copy the payload into @p buffer.
 *
 * @param buffer       Destination buffer.
 * @param buffer_len   Capacity of @p buffer in bytes.
 * @param received_len Updated with the total number of bytes stored.
 * @param timeout_ms   Overall deadline in milliseconds.
 * @param mode         WIFI_IPD_DISCARD_OVERSIZE (UDP) or
 *                     WIFI_IPD_ACCUMULATE (TCP).
 * @return Number of bytes received on success, or -1 on error/timeout.
 */
static int wifi_receive_ipd(uint8_t     *buffer,
                                uint32_t     buffer_len,
                                uint32_t     timeout_ms,
                                WIFI_IPD_Mode mode)
{
    typedef enum { WAIT_IPD, READ_LEN, READ_DATA } State;

    State    state        = WAIT_IPD;
    uint32_t matched      = 0U;
    uint32_t frame_len    = 0U;
    uint32_t frame_count  = 0U;
    uint32_t total        = 0U;
    uint32_t start;
    unsigned char ch;

    if ((buffer == NULL) || (buffer_len == 0U)) {
        return -1;
    }

    start = Get_Tick();

    while (DELAY_GetElapsedMillis(start) < timeout_ms) {
        while (UART_GetChar(&ch)) {
            switch (state) {

                case WAIT_IPD:
                    if (wifi_match_seq((char)ch, "+IPD,", &matched)) {
                        frame_len   = 0U;
                        frame_count = 0U;
                        state       = READ_LEN;
                    }
                    break;

                case READ_LEN:
                    if ((ch >= '0') && (ch <= '9')) {
                        frame_len = (frame_len * 10U) + (uint32_t)(ch - '0');
                    } else if (ch == ':') {
                        if (frame_len == 0U) {
                            state   = WAIT_IPD;
                            matched = 0U;
                            break;
                        }

                        if (mode == WIFI_IPD_DISCARD_OVERSIZE) {
                            if (frame_len > buffer_len) {
                                state   = WAIT_IPD;
                                matched = 0U;
                                break;
                            }
                            total = 0U;
                        }

                        frame_count = 0U;
                        state       = READ_DATA;
                    } else {
                        state   = WAIT_IPD;
                        matched = 0U;
                    }
                    break;

                case READ_DATA:
                    frame_count++;

                    if (total < buffer_len) {
                        buffer[total++] = ch;
                    }

                    if (frame_count >= frame_len) {
                        if (mode == WIFI_IPD_DISCARD_OVERSIZE) {
                            return total;
                        }

                        if (total >= buffer_len) {
                            return total;
                        }

                        state   = WAIT_IPD;
                        matched = 0U;
                    }
                    break;

                default:
                    state   = WAIT_IPD;
                    matched = 0U;
                    break;
            }
        }
    }

    /* Timeout: return accumulated bytes for TCP, -1 for UDP */
    if ((mode == WIFI_IPD_ACCUMULATE) && (total > 0U)) {
        return total;
    }

    return -1;
}

/* -------------------------------------------------------------------------
 * Public receive functions
 * ------------------------------------------------------------------------- */

int WIFI_ReceiveUDP(uint8_t  *buffer,
                        uint32_t  buffer_len,
                        uint32_t  timeout_ms)
{
    return wifi_receive_ipd(buffer, buffer_len, timeout_ms,
                            WIFI_IPD_DISCARD_OVERSIZE);
}

int WIFI_ReceiveTCP(uint8_t  *buffer,
                        uint32_t  buffer_len,
                        uint32_t  timeout_ms)
{
    return wifi_receive_ipd(buffer, buffer_len, timeout_ms,
                            WIFI_IPD_ACCUMULATE);
}
