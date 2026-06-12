/**
 * @file transport.h
 * @brief This header file provides the transport adapter API used by MQTTPacket.
 * @version 1
 * @date 13 May 2026
 * @author IBM Corp. / Grupo 08
 *
 */

/**
 * @defgroup transport_module  Transport Module
 * @ingroup free_rtos
 * @brief Minimal transport abstraction layer.
 * @{
 */

#ifndef TRANSPORT_H_
#define TRANSPORT_H_

/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - media specifics, nice api doc :^)
 *******************************************************************************/

/**
 * @brief I/O function table used by the transport layer.
 *
 * The MQTT transport code uses these callbacks to send and receive raw bytes
 * over the active network interface.
 */
typedef struct {
    /**
     * @brief Sends bytes through the active transport.
     *
     * @param address Pointer to the data buffer to send.
     * @param bytes Number of bytes to send.
     * @return Number of bytes actually sent, or TRANSPORT_ERROR on error.
     */
    int (*send)(unsigned char *address, unsigned int bytes);

    /**
     * @brief Receives bytes from the active transport.
     *
     * @param address Destination buffer.
     * @param maxbytes Maximum number of bytes to copy.
     * @return Number of bytes copied, 0 if no data is available, or
     *         TRANSPORT_ERROR on error.
     */
    int (*recv)(unsigned char *address, unsigned int maxbytes);
} transport_iofunctions_t;

/** @brief Non-blocking transport operation completed successfully. */
#define TRANSPORT_DONE  1

/** @brief Non-blocking transport operation must be called again. */
#define TRANSPORT_AGAIN 0

/** @brief Transport operation failed. */
#define TRANSPORT_ERROR -1

/**
 * @brief Sends a complete  packet.
 *
 * Blocks internally until all requested bytes have been sent or an error is
 * reported by the configured send callback.
 *
 * @param sock Transport identifier returned by transport_open().
 * @param buf Pointer to the packet buffer to send.
 * @param buflen Number of bytes to send.
 *
 * @return @p buflen on success.
 * @return TRANSPORT_ERROR on error.
 */
int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen);

/**
 * @brief Blocking receive function placeholder.
 *
 * This function is kept for API compatibility with the MQTTPacket examples but
 * is not implemented in this project. Use transport_getdatanb() instead.
 *
 * @param buf Destination buffer.
 * @param count Number of bytes requested.
 *
 * @return TRANSPORT_ERROR because the function is not supported.
 *
 * @warning This function is not supported in this implementation.
 */
int transport_getdata(unsigned char* buf, int count);

/**
 * @brief Starts a non-blocking packet send operation.
 *
 * Stores the buffer pointer and byte count used by subsequent calls to
 * transport_sendPacketBuffernb().
 *
 * @param sock Transport identifier returned by transport_open().
 * @param buf Pointer to the packet buffer to send.
 * @param buflen Number of bytes to send.
 */
void transport_sendPacketBuffernb_start(int sock, unsigned char* buf, int buflen);

/**
 * @brief Continues a non-blocking packet send operation.
 *
 * Must be called repeatedly after transport_sendPacketBuffernb_start() until it
 * returns TRANSPORT_DONE or TRANSPORT_ERROR.
 *
 * @param sock Transport identifier returned by transport_open().
 *
 * @return TRANSPORT_DONE when all bytes were sent.
 * @return TRANSPORT_AGAIN when the function must be called again.
 * @return TRANSPORT_ERROR on error.
 */
int transport_sendPacketBuffernb(int sock);

/**
 * @brief Non-blocking receive function used by MQTTPacket_readnb().
 *
 * Calls the configured receive callback and returns immediately with the number
 * of bytes currently available.
 *
 * @param sck Pointer to the transport identifier.
 * @param buf Destination buffer.
 * @param count Maximum number of bytes to receive.
 *
 * @return Number of bytes read.
 * @return 0 if no data is currently available.
 * @return TRANSPORT_ERROR on error.
 */
int transport_getdatanb(void *sck, unsigned char* buf, int count);

/**
 * @brief Opens the transport abstraction using the supplied I/O functions.
 *
 * The actual hardware/network connection must already be established by the
 * caller. This function only stores the callback table used by the transport
 * layer.
 *
 * @param thisio Pointer to the I/O callback table.
 *
 * @return Transport identifier on success.
 * @return TRANSPORT_ERROR on error.
 */
int transport_open(transport_iofunctions_t *thisio);

/**
 * @brief Closes the transport abstraction.
 *
 * @param sock Transport identifier returned by transport_open().
 *
 * @return TRANSPORT_DONE on success.
 */
int transport_close(int sock);

#endif /* TRANSPORT_H_ */

/** @} */
