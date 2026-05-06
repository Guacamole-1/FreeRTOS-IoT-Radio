#ifndef NTP_H_
#define NTP_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NTP_SERVER_DEFAULT       "pool.ntp.org"
#define NTP_PORT                 123U
#define NTP_PACKET_SIZE          48U
#define NTP_TIMEOUT_MS           10000U
#define NTP_UNIX_EPOCH_OFFSET    2208988800UL

typedef struct {
    uint32_t unix_time;
    uint32_t ntp_seconds;
    uint32_t ntp_fraction;
} NTP_Time;

void NTP_BuildRequest(uint8_t packet[NTP_PACKET_SIZE]);
bool NTP_ParseResponse(const uint8_t packet[NTP_PACKET_SIZE], NTP_Time *time_out);
bool NTP_GetTime(const char *server, NTP_Time *time_out);
bool NTP_GetUnixTime(uint32_t *unix_time_out);

#ifdef __cplusplus
}
#endif

#endif /* NTP_H_ */
