#ifndef WIFI_H_
#define WIFI_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_DEFAULT_BAUDRATE      115200U
#define WIFI_DEFAULT_TIMEOUT_MS    5000U
#define WIFI_RX_BUFFER_SIZE        512U

typedef enum {
    WIFI_OK = 0,
    WIFI_ERROR,
    WIFI_TIMEOUT,
    WIFI_INVALID_ARG,
    WIFI_SEND_ERROR,
    WIFI_NOT_CONNECTED
} WIFI_Status;

typedef enum {
    WIFI_MODE_STATION = 1,
    WIFI_MODE_SOFTAP = 2,
    WIFI_MODE_STATION_SOFTAP = 3
} WIFI_Mode;

bool WIFI_Init(void);
bool WIFI_Test(void);
bool WIFI_Reset(void);
bool WIFI_SetModeStation(void);
bool WIFI_SetMode(WIFI_Mode mode);
bool WIFI_ListAPs(char *response, uint32_t response_len);
bool WIFI_ConnectAP(const char *ssid, const char *password);
bool WIFI_GetStatus(char *response, uint32_t response_len);
bool WIFI_OpenTCP(const char *ip, uint16_t port);
bool WIFI_OpenUDP(const char *host, uint16_t port);
bool WIFI_Send(const uint8_t *data, uint32_t len);
bool WIFI_ReceiveUDP(uint8_t *buffer, uint32_t buffer_len, uint32_t *received_len, uint32_t timeout_ms);
bool WIFI_Close(void);

WIFI_Status WIFI_SendATCommand(const char *cmd,
                               const char *expected,
                               char *response,
                               uint32_t response_len,
                               uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_H_ */
