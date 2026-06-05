/*
 * dummypub.c
 *
 *  Created on: 2 Jun 2020
 *      Author: psampaio
 */
#include "Delay.h"
#include "FreeRTOS.h"
#include "MQTTPacket.h"
#include "MQTTSubscribe.h"
#include "Radio.h"
#include "portable.h"
#include "projdefs.h"
#include "task.h"
#include "Wifi.h"
#include "transport.h"
#include "wifi_rtos.h"
#include "base.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types.h>
#include "publisher.h"

#define CONNECTION_KEEPALIVE_S 	60UL
#define MQTT_ADDRESS			"iot-ps.ddns.net"
#define MQTT_PORT				1884
#define MQTT_DEVICE_TOKEN		"SEIOT-2025-2026-G08"
#define RECV_BUF_SIZE WIFI_RX_BUFFER_SIZE
#define RECV_BUF_TIMEOUT WIFI_DEFAULT_TIMEOUT_MS
static uint8_t _recv_buf[RECV_BUF_SIZE];		// circular buffer to store received wifi data
static unsigned int _recv_buf_p;				// pointer to current position in buffer
static unsigned int _recv_buf_len;				// len of bytes currently buffered

static void cpy_recv_buffer(uint8_t* dst, unsigned int count){

	if (count > _recv_buf_len) {
        count = _recv_buf_len;
    }

	for (int i = 0; i < count ; i++) {
		dst[i] = _recv_buf[(_recv_buf_p + i) % RECV_BUF_SIZE];
	}
	_recv_buf_p = (_recv_buf_p + count) % RECV_BUF_SIZE;
	_recv_buf_len -= count;
}
static void write_recv_buffer(uint8_t* src, unsigned int count){

	if (count > (RECV_BUF_SIZE - _recv_buf_len)) {
        count = RECV_BUF_SIZE - _recv_buf_len;
    }

	for (int i = 0; i < count; i++) {
		_recv_buf[(_recv_buf_p + _recv_buf_len + i) % RECV_BUF_SIZE] = src[i];
	}
	_recv_buf_len += count;
}
/**
 *  gets size bytes from receive buffer or the remaining bytes to empty it
 *  if buffer is empty, it tries to get Tcp data
 * @returns the amount of bytes written onto address
 */

static int get_recv_buffer(uint8_t * address, unsigned int size) {
	int recvd;
	uint8_t temp_buf[RECV_BUF_SIZE];

	if (_recv_buf_len > 0) { // if there is still something in the buffer, send whats left
		//bool send_all = size > _recv_buf_len;
		//int ret = send_all ? _recv_buf_len : size;
		//_recv_buf_len = send_all ? 0 : (_recv_buf_len - size) ;
		unsigned int ret = (size > _recv_buf_len) ? _recv_buf_len : size;
		cpy_recv_buffer(address, ret);
		return ret;
	}
	// try to get wifi data
	recvd = WIFI_RTOS_ReceiveTcp(temp_buf, RECV_BUF_SIZE, RECV_BUF_TIMEOUT);

	if (recvd <= 0) {
        return 0;
    }
	//write to address with fresh data
    write_recv_buffer(temp_buf, (unsigned int)recvd);

    unsigned int ret = (size > _recv_buf_len) ? _recv_buf_len : size;
    cpy_recv_buffer(address, ret);

    return (int)ret;
}


base_t NETWORK_Init(void) {
	// wifi is already initialized in task_app

	return SUCCESS;
}

base_t NETWORK_Connect(const char *host, const unsigned short int port,
					   const unsigned short int keepalive) {

	return WIFI_RTOS_OpenTcp(host,port,keepalive);
}

int NETWORK_Send(unsigned char *address, unsigned int bytes) {
	return WIFI_RTOS_Send(address,bytes);
}

int NETWORK_Recv(unsigned char *address, unsigned int maxbytes) {
	return get_recv_buffer(address, maxbytes);
}

base_t NETWORK_Close(){
	return WIFI_RTOS_Close();
}

static void Publisher_Task(void *pvParameters);
// echo.iot-ps.ddns.net
// GET /

typedef enum {
	STATE_INIT,
	STATE_CONNECT,
	STATE_WAIT_CONNECT,
	STATE_SUBSCRIBE,
	STATE_SUBSCRIBE_SUBACK,
	STATE_SUBSCRIBE_RECEIVE,
	STATE_PUBLISH,
} states ;

base_t Start_Publisher_Task(void *pvParameters){
	if (xTaskCreate(Publisher_Task, "App", configMINIMAL_STACK_SIZE * 4,
					pvParameters, tskIDLE_PRIORITY + 1, NULL) != pdPASS){
        return ERROR;
    }
	return SUCCESS;
}

static void Publisher_Task(void *pvParameters) {
	unsigned char buffer[256];
	MQTTTransport transporter;
	int result;
	int length;
	uint32_t start_tick = 0;
	Radio_Data* radio_data = ( Radio_Data *)pvParameters;
	if ((result = NETWORK_Init())) {
		printf("Network fail: %d\n",result);
		return;
	}

	// ESP8266 Transport Layer
	static transport_iofunctions_t iof = { NETWORK_Send, NETWORK_Recv };
	int transport_socket = transport_open(&iof);

	int state = STATE_INIT;
	while (true) {
		switch (state) {
		case STATE_INIT:

			NETWORK_Close();

			if (!NETWORK_Connect(MQTT_ADDRESS, MQTT_PORT, CONNECTION_KEEPALIVE_S)) {
				state++;
			}
			vTaskDelay(pdMS_TO_TICKS(1000));
			break;
		case STATE_CONNECT:
			transporter.sck = &transport_socket;
			transporter.getfn = transport_getdatanb;
			transporter.state = 0;

			MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
			connectData.MQTTVersion = 3;
			connectData.username.cstring = MQTT_DEVICE_TOKEN;
			connectData.clientID.cstring = MQTT_DEVICE_TOKEN;
			connectData.keepAliveInterval = CONNECTION_KEEPALIVE_S * 2;
			length = MQTTSerialize_connect(buffer, sizeof(buffer), &connectData);
			// Send CONNECT to the mqtt broker.
			if ((result = transport_sendPacketBuffer(transport_socket, buffer, length)) == length) {
				state++;
			} else {
				state = STATE_INIT;
			}
			break;
		case STATE_WAIT_CONNECT:
			// Wait for CONNACK response from the MQTT broker.
			while (true) {
				if ((result = MQTTPacket_readnb(buffer, sizeof(buffer), &transporter)) == CONNACK) {
					// Check if the connection was accepted.
					unsigned char sessionPresent, connack_rc;
					if ((MQTTDeserialize_connack(&sessionPresent, &connack_rc, buffer,
												 sizeof(buffer)) != 1) || (connack_rc != 0)) {
						state = STATE_INIT;
						break;
					} else {
						state++;
						break;
					}
				} else if (result == -1) {
					state = STATE_INIT;
					break;
				}
			}
			break;
		case STATE_SUBSCRIBE:
			{
				MQTTString subtopic = MQTTString_initializer;
				subtopic.cstring = "v1/devices/me/attributes";
				int req_qos = 0;
				length = MQTTSerialize_subscribe(buffer, sizeof(buffer),0,1,1,&subtopic,&req_qos);
				result = transport_sendPacketBuffer(transport_socket, buffer, length);
				state  = (result == length) ? STATE_SUBSCRIBE_SUBACK : STATE_INIT;
				break;
			}
		case STATE_SUBSCRIBE_SUBACK:
			{
				result = MQTTPacket_readnb(buffer,sizeof(buffer), &transporter);
				if(result == SUBACK){
					int granted_qos;
					int count = 0;
					unsigned short packet_id;

					MQTTDeserialize_suback(&packet_id, 1, &count, &granted_qos, buffer, sizeof(buffer));

					/* 0x80 means the broker rejected the subscription */
					state = (granted_qos != 0x80) ? STATE_SUBSCRIBE_RECEIVE : STATE_INIT;
				}else if (result == -1) {
					state = STATE_INIT;
				}
				// result == 0 -> try again
				vTaskDelay(pdMS_TO_TICKS(10));
				break;

			}
		case STATE_SUBSCRIBE_RECEIVE:
			{
				result = MQTTPacket_readnb(buffer,sizeof(buffer), &transporter);
				if (result == PUBLISH) {
					unsigned char dup;
					int qos;
					unsigned char retained;
					unsigned short packetId;
					MQTTString receivedTopic;
					unsigned char *payload;
					int payloadLen;

					if (MQTTDeserialize_publish(&dup,
												&qos,
												&retained,
												&packetId,
												&receivedTopic,
												&payload,
												&payloadLen,
												buffer,
												sizeof(buffer)) == 1) {

						printf("MQTT message received\n");

						printf("Topic: %.*s\n",
							   receivedTopic.lenstring.len,
							   receivedTopic.lenstring.data);

						printf("Payload: %.*s\n",
							   payloadLen,
							   payload);
					}

				}else if (result < 0) {
					state = STATE_INIT;
				} else {
					// No full packet yet
					vTaskDelay(pdMS_TO_TICKS(100));
					state = STATE_PUBLISH;
					break;
				}
				break;
			}
		case STATE_PUBLISH:
			if(DELAY_GetElapsedMillis(start_tick) >= 30000){
				MQTTString topicString = MQTTString_initializer;
				topicString.cstring = "v1/devices/me/telemetry";
				unsigned char payload[64];
				length = sprintf((char*)payload, "{\"volume\":%d, \"station\":%f}", radio_data->volume, CURRENT_FREQ( radio_data->channel));
				length = MQTTSerialize_publish(buffer, sizeof(buffer), 0, 0, 0, 0, topicString, payload, length);
				// Send PUBLISH to the MQTT broker.
				if ((result = transport_sendPacketBuffer(transport_socket, buffer, length)) == length) {
					// Wait for new data, wait or delay.
					// vTaskDelay(pdMS_TO_TICKS(30000));
					start_tick = Get_Tick();
					vTaskDelay(pdMS_TO_TICKS(100));
					state = STATE_SUBSCRIBE_RECEIVE;

				} else {
					state = STATE_INIT;
				}
			} else {
				state = STATE_SUBSCRIBE_RECEIVE;
				vTaskDelay(pdMS_TO_TICKS(100));
			}
			break;
		default:
			state = STATE_INIT;
		}
	}
}
