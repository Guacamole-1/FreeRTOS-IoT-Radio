#include "NTP.h"

#include <string.h>

#include "Wifi.h"

static uint32_t ntp_read_u32_be(const uint8_t *data) {
    return ((uint32_t)data[0] << 24) |
           ((uint32_t)data[1] << 16) |
           ((uint32_t)data[2] << 8)  |
           ((uint32_t)data[3]);
}

void NTP_BuildRequest(uint8_t packet[NTP_PACKET_SIZE]) {
    memset(packet, 0, NTP_PACKET_SIZE);
    packet[0] = 0xE3U;
    packet[1] = 0U;
    packet[2] = 6U;
    packet[3] = 0xECU;
}

bool NTP_ParseResponse(const uint8_t packet[NTP_PACKET_SIZE], NTP_Time *time_out) {
    uint32_t tx_seconds;
    uint32_t tx_fraction;

    if ((packet == NULL) || (time_out == NULL)) {
        return false;
    }

    tx_seconds = ntp_read_u32_be(&packet[40]);
    tx_fraction = ntp_read_u32_be(&packet[44]);

    if (tx_seconds < NTP_UNIX_EPOCH_OFFSET) {
        return false;
    }

    time_out->ntp_seconds = tx_seconds;
    time_out->ntp_fraction = tx_fraction;
    time_out->unix_time = tx_seconds - NTP_UNIX_EPOCH_OFFSET;

    return true;
}

bool NTP_GetTime(const char *server, NTP_Time *time_out) {
    uint8_t request[NTP_PACKET_SIZE];
    uint8_t response[NTP_PACKET_SIZE];
    uint32_t received_len = 0U;
    const char *server_to_use = (server != NULL) ? server : NTP_SERVER_DEFAULT;

    if (time_out == NULL) {
        return false;
    }

    NTP_BuildRequest(request);

    if (!WIFI_OpenUDP(server_to_use, NTP_PORT)) {
        return false;
    }

    if (!WIFI_Send(request, NTP_PACKET_SIZE)) {
        WIFI_Close();
        return false;
    }

    if (!WIFI_ReceiveUDP(response, sizeof(response), &received_len, NTP_TIMEOUT_MS)) {
        WIFI_Close();
        return false;
    }

    WIFI_Close();

    if (received_len < NTP_PACKET_SIZE) {
        return false;
    }

    return NTP_ParseResponse(response, time_out);
}

bool NTP_GetUnixTime(uint32_t *unix_time_out) {
    NTP_Time time;

    if (unix_time_out == NULL) {
        return false;
    }

    if (!NTP_GetTime(NTP_SERVER_DEFAULT, &time)) {
        return false;
    }

    *unix_time_out = time.unix_time;
    return true;
}
