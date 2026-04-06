/**
 * @file display.h
 * @brief This header file provides the API for controlling the LCD Display with
 * concurrent access
 * @version 1
 * @date 11 Mar 2026
 * @author Grupo 08
 *
 */


/**
 * @defgroup free_rtos FreeRTOS Modules
 * @brief Modules built on top of FreeRTOS primitives.
 *
 * This group contains queue-based, task-based, and synchronization
 * modules that use FreeRTOS services.
 *
 * @{
 */

/**
 * @defgroup display_module Display Module
 * @ingroup free_rtos
 * @brief Asynchronous LCD display manager.
 * @{
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "base.h"
#define DISPLAY_MAX_QUEUE 10
#define DISPLAY_TICKS_TO_WAIT portMAX_DELAY


typedef enum {
	WRITE_STR,
	CLEAR,
	CURSOR_SET,
	WRITE_CMD
} DISPLAY_type;

typedef struct {
	DISPLAY_type id;
	void* args;
} DISPLAY_Item;

typedef struct {
	char* str;
} DISPLAY_Args_WS;



/**
 * @brief Initializes the display subsystem.
 *
 * Initializes:
 * - delay driver
 * - LCD hardware
 * - internal display queue
 *
 * Must be called before any other display function.
 *
 * @return DISP_SUCCESS on success
 * @return DISP_INIT_ERROR if LCD/delay initialization fails
 * @return DISP_QUEUE_ERROR if queue creation fails
 */
base_t DISPLAY_Init(void);

/**
 * @brief Sends a display request asynchronously.
 *
 * The item is deep-copied internally before being placed on the queue.
 *
 * Safe to reuse stack-based variables after this call returns.
 *
 * @param item Display message to send
 *
 * @return DISP_SUCCESS on success
 * @return DISP_QUEUE_FULL if queue is full
 */
base_t DISPLAY_Send(DISPLAY_Item item);

/**
 * @brief Receives a display item from the queue.
 *
 * Intended mainly for internal use by DISPLAY_Manager().
 *
 * @param recvd_item Pointer that receives the dequeued item pointer
 *
 * @return DISP_SUCCESS if an item was received
 * @return DISP_QUEUE_EMPTY if no item was available
 */
base_t DISPLAY_Receive(DISPLAY_Item **recvd_item);

/**
 * @brief Queues a formatted string for display.
 *
 * Uses printf-style formatting and sends the resulting string
 * as a WRITE_STR message.
 *
 * @param fmt Format string
 * @param ... Variadic arguments
 */
base_t DISPLAY_Printf(const char *fmt, ...);

/**
 * @brief Main display processing loop.
 *
 * This function is intended to run inside a dedicated FreeRTOS task.
 * It continuously processes queued display messages.
 *
 * Normally does not return.
 *
 * @return Never normally returns
 */
base_t DISPLAY_Manager(void);

#endif /* DISPLAY_H_ */

/** @} */

/** @} */
