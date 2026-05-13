/*
 * dummypub.c
 *
 *  Created on: 2 Jun 2020
 *      Author: psampaio
 */

#include "MQTTPacket.h"
#include "transport.h"

bool NETWORK_Init(void) {
	// Init Network
}

bool NETWORK_Connect(const char *host, const unsigned short int port,
		const unsigned short int keepalive) {
			// Connect to host
}

int NETWORK_Send(unsigned char *address, unsigned int bytes) {
	// Send Packet
}

int NETWORK_Recv(unsigned char *address, unsigned int maxbytes) {
	// Receive packet
}

#define CONNECTION_KEEPALIVE_S 	60UL
#define MQTT_ADDRESS			"iot-ps.ddns.net"
#define MQTT_PORT				1884
#define MQTT_DEVICE_TOKEN		"SE2-G#-RADIO"

static void DummyPublisher_Task(void *pvParameters) {
	unsigned char buffer[128];
	MQTTTransport transporter;
	int result;
	int length;

	if (!NETWORK_Init()) {
		printf("Network fail\n");
		return;
	}

	// ESP8266 Transport Layer
	static transport_iofunctions_t iof = { NETWORK_Send, NETWORK_Recv };
	int transport_socket = transport_open(&iof);

	int state = STATE_INIT;
	while (true) {
		switch (state) {
		case STATE_INIT:
			if (NETWORK_Connect(MQTT_ADDRESS, MQTT_PORT, CONNECTION_KEEPALIVE_S)) {
				state++;
			}
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
		case STATE_PUBLISH:
			MQTTString topicString = MQTTString_initializer;
			topicString.cstring = "v1/devices/me/telemetry";
			unsigned char payload[64];
			length = sprintf(payload, "{\"vol\":%d, \"freq\":%f}", volume, frequency);
			length = MQTTSerialize_publish(buffer, sizeof(buffer), 0, 0, 0, 0, topicString, payload, length);
			// Send PUBLISH to the MQTT broker.
			if ((result = transport_sendPacketBuffer(transport_socket, buffer, length)) == length) {
				// Wait for new data.
			} else {
				state = STATE_INIT;
			}
			break;
		default:
			state = STATE_INIT;
		}
	}
}
