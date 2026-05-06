#include "Wifi.h"

#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "Delay.h"

#define WIFI_CMD_BUFFER_SIZE 128U

static bool wifi_initialized = false;

static void wifi_store_char(char *buffer, uint32_t buffer_len, uint32_t *index, char c) {
    if ((buffer != NULL) && (buffer_len > 0U) && (*index < (buffer_len - 1U))) {
        buffer[*index] = c;
        (*index)++;
        buffer[*index] = '\0';
    }
}

static bool wifi_response_contains(const char *response, const char *token) {
    return (response != NULL) && (token != NULL) && (strstr(response, token) != NULL);
}

WIFI_Status WIFI_SendATCommand(const char *cmd,
                               const char *expected,
                               char *response,
                               uint32_t response_len,
                               uint32_t timeout_ms) {
    uint32_t start;
    uint32_t index = 0U;
    unsigned char ch;

    char internal_response[WIFI_CMD_BUFFER_SIZE];

    char *active_response;
    uint32_t active_response_len;

    if (cmd == NULL) {
        return WIFI_INVALID_ARG;
    }

    /*
     * Se o utilizador passou um buffer, usamos esse buffer.
     * Se passou NULL, usamos um buffer interno temporario.
     *
     * Isto e importante porque precisamos sempre de guardar a resposta
     * para conseguir procurar "OK", "ERROR", "FAIL", ">" ou "SEND OK".
     */
    if ((response != NULL) && (response_len > 0U)) {
        active_response = response;
        active_response_len = response_len;
    } else {
        active_response = internal_response;
        active_response_len = sizeof(internal_response);
    }

    active_response[0] = '\0';

    UART_FlushRx();

    /*
     * Pode ser string vazia.
     * Isto acontece em WIFI_Send(), quando ja enviamos os dados
     * e so queremos esperar por "SEND OK".
     */
    if (cmd[0] != '\0') {
        UART_WriteString(cmd);
    }

    start = Get_Tick();

    while (DELAY_GetElapsedMillis(start) < timeout_ms) {
        while (UART_GetChar(&ch)) {
            wifi_store_char(active_response,
                            active_response_len,
                            &index,
                            (char)ch);

            if ((expected != NULL) &&
                wifi_response_contains(active_response, expected)) {
                return WIFI_OK;
            }

            if (wifi_response_contains(active_response, "ERROR") ||
                wifi_response_contains(active_response, "FAIL")) {
                return WIFI_ERROR;
            }
        }
    }

    return WIFI_TIMEOUT;
}

bool WIFI_Init(void) {
    if (!wifi_initialized) {
        if (!UART_Initialize(WIFI_DEFAULT_BAUDRATE)) {
            return false;
        }

        DELAY_Milliseconds(500U);
        UART_FlushRx();

        wifi_initialized = true;
    }

    if (WIFI_Test()) {
        return true;
    }

    DELAY_Milliseconds(500U);
    UART_FlushRx();

    return WIFI_Test();
}

bool WIFI_Test(void) {
    return WIFI_SendATCommand("AT\r\n", "OK", NULL, 0U, 1000U) == WIFI_OK;
}

bool WIFI_Reset(void) {
    WIFI_Status status;

    status = WIFI_SendATCommand("AT+RST\r\n", "OK", NULL, 0U, 2000U);
    DELAY_Milliseconds(3000U);

    return (status == WIFI_OK) || WIFI_Test();
}

bool WIFI_SetMode(WIFI_Mode mode) {
    char cmd[WIFI_CMD_BUFFER_SIZE];

    if ((mode < WIFI_MODE_STATION) || (mode > WIFI_MODE_STATION_SOFTAP)) {
        return false;
    }

    snprintf(cmd, sizeof(cmd), "AT+CWMODE=%d\r\n", (int)mode);
    return WIFI_SendATCommand(cmd, "OK", NULL, 0U, WIFI_DEFAULT_TIMEOUT_MS) == WIFI_OK;
}

bool WIFI_SetModeStation(void) {
    return WIFI_SetMode(WIFI_MODE_STATION);
}

bool WIFI_ListAPs(char *response, uint32_t response_len) {
    return WIFI_SendATCommand("AT+CWLAP\r\n", "OK", response, response_len, 10000U) == WIFI_OK;
}

bool WIFI_ConnectAP(const char *ssid, const char *password) {
    char cmd[WIFI_CMD_BUFFER_SIZE];

    if ((ssid == NULL) || (password == NULL)) {
        return false;
    }

    snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);
    return WIFI_SendATCommand(cmd, "OK", NULL, 0U, 20000U) == WIFI_OK;
}

bool WIFI_GetStatus(char *response, uint32_t response_len) {
    return WIFI_SendATCommand("AT+CIPSTATUS\r\n", "OK", response, response_len, WIFI_DEFAULT_TIMEOUT_MS) == WIFI_OK;
}

bool WIFI_OpenTCP(const char *ip, uint16_t port) {
    char cmd[WIFI_CMD_BUFFER_SIZE];

    if (ip == NULL) {
        return false;
    }

    WIFI_SendATCommand("AT+CIPMUX=0\r\n", "OK", NULL, 0U, WIFI_DEFAULT_TIMEOUT_MS);

    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"TCP\",\"%s\",%u\r\n", ip, (unsigned int)port);
    return WIFI_SendATCommand(cmd, "OK", NULL, 0U, 10000U) == WIFI_OK;
}

bool WIFI_OpenUDP(const char *host, uint16_t port) {
    char cmd[WIFI_CMD_BUFFER_SIZE];

    if (host == NULL) {
        return false;
    }

    WIFI_SendATCommand("AT+CIPMUX=0\r\n", "OK", NULL, 0U, WIFI_DEFAULT_TIMEOUT_MS);

    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"UDP\",\"%s\",%u\r\n", host, (unsigned int)port);
    return WIFI_SendATCommand(cmd, "OK", NULL, 0U, 10000U) == WIFI_OK;
}

bool WIFI_Send(const uint8_t *data, uint32_t len) {
    char cmd[WIFI_CMD_BUFFER_SIZE];

    if ((data == NULL) || (len == 0U)) {
        return false;
    }

    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%lu\r\n", (unsigned long)len);

    if (WIFI_SendATCommand(cmd, ">", NULL, 0U, WIFI_DEFAULT_TIMEOUT_MS) != WIFI_OK) {
        return false;
    }

    UART_WriteBuffer(data, len);

    return WIFI_SendATCommand("", "SEND OK", NULL, 0U, 10000U) == WIFI_OK;
}


static bool wifi_match_sequence(char received, const char *sequence, uint32_t *matched) {
    if (received == sequence[*matched]) {
        (*matched)++;
        return sequence[*matched] == '\0';
    }

    *matched = (received == sequence[0]) ? 1U : 0U;
    return false;
}

bool WIFI_ReceiveUDP(uint8_t *buffer, uint32_t buffer_len, uint32_t *received_len, uint32_t timeout_ms) {
    uint32_t start;
    uint32_t matched = 0U;
    uint32_t expected_len = 0U;
    uint32_t count = 0U;
    unsigned char ch;
    enum { WAIT_IPD, READ_LEN, READ_DATA } state = WAIT_IPD;

    if ((buffer == NULL) || (buffer_len == 0U) || (received_len == NULL)) {
        return false;
    }

    *received_len = 0U;
    start = Get_Tick();

    while (DELAY_GetElapsedMillis(start) < timeout_ms) {
        while (UART_GetChar(&ch)) {
            switch (state) {
                case WAIT_IPD:
                    if (wifi_match_sequence((char)ch, "+IPD,", &matched)) {
                        expected_len = 0U;
                        state = READ_LEN;
                    }
                    break;

                case READ_LEN:
                    if ((ch >= '0') && (ch <= '9')) {
                        expected_len = (expected_len * 10U) + (uint32_t)(ch - '0');
                    } else if (ch == ':') {
                        if ((expected_len == 0U) || (expected_len > buffer_len)) {
                            return false;
                        }
                        count = 0U;
                        state = READ_DATA;
                    } else {
                        state = WAIT_IPD;
                        matched = 0U;
                    }
                    break;

                case READ_DATA:
                    buffer[count++] = ch;
                    if (count >= expected_len) {
                        *received_len = count;
                        return true;
                    }
                    break;

                default:
                    state = WAIT_IPD;
                    matched = 0U;
                    break;
            }
        }
    }

    return false;
}

bool WIFI_Close(void) {
    return WIFI_SendATCommand("AT+CIPCLOSE\r\n", "OK", NULL, 0U, WIFI_DEFAULT_TIMEOUT_MS) == WIFI_OK;
}
