#ifndef WIFI_RTOS_H_
#define WIFI_RTOS_H_

#include <stdint.h>
#include "base.h"
#include "Wifi.h"

base_t WIFI_RTOS_INIT(void);
base_t WIFI_RTOS_TEST(void);
base_t WIFI_RTOS_RESET(void);
base_t WIFI_RTOS_SET_MODE_STATION(void);
base_t WIFI_RTOS_CONNECT_AP(const char *ssid, const char *password);
base_t WIFI_RTOS_GET_STATUS(char *response, uint32_t response_len);
base_t WIFI_RTOS_OPEN_TCP(const char *ip, uint16_t port);
base_t WIFI_RTOS_OPEN_UDP(const char *host, uint16_t port);
base_t WIFI_RTOS_SEND(const uint8_t *data, uint32_t len);
base_t WIFI_RTOS_CLOSE(void);

#endif /* WIFI_RTOS_H_ */
