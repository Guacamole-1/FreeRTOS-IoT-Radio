#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "wifi_rtos.h"
#include "Wifi.h"
#include "mutex_wrapper.h"

LOCK_DEF;

static base_t wifi_bool_to_base(bool result) {
    return result ? SUCCESS : ERROR;
}

base_t WIFI_RTOS_INIT(void) {
    bool result;

    INIT_NCHECK();
    LOCK_INIT();

    LOCK()
        result = WIFI_Init();
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_TEST(void) {
    bool result;

    INIT_CHECK();

    LOCK()
        result = WIFI_Test();
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_RESET(void) {
    bool result;

    INIT_CHECK();

    LOCK()
        result = WIFI_Reset();
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_SET_MODE_STATION(void) {
    bool result;

    INIT_CHECK();

    LOCK()
        result = WIFI_SetModeStation();
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_CONNECT_AP(const char *ssid, const char *password) {
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

base_t WIFI_RTOS_GET_STATUS(char *response, uint32_t response_len) {
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

base_t WIFI_RTOS_OPEN_TCP(const char *ip, uint16_t port) {
    bool result;

    INIT_CHECK();

    if ((ip == NULL) || (port == 0U)) {
        return ARG_ERROR;
    }

    LOCK()
        result = WIFI_OpenTCP(ip, port);
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_OPEN_UDP(const char *host, uint16_t port) {
    bool result;

    INIT_CHECK();

    if ((host == NULL) || (port == 0U)) {
        return ARG_ERROR;
    }

    LOCK()
        result = WIFI_OpenUDP(host, port);
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_SEND(const uint8_t *data, uint32_t len) {
    bool result;

    INIT_CHECK();

    if ((data == NULL) || (len == 0U)) {
        return ARG_ERROR;
    }

    LOCK()
        result = WIFI_Send(data, len);
    UNLOCK()

    return wifi_bool_to_base(result);
}

base_t WIFI_RTOS_CLOSE(void) {
    bool result;

    INIT_CHECK();

    LOCK()
        result = WIFI_Close();
    UNLOCK()

    return wifi_bool_to_base(result);
}
