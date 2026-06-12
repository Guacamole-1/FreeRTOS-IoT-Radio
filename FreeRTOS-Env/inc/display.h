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
#include "stdint.h"
#define DISPLAY_MAX_QUEUE 10
#define DISPLAY_TICKS_TO_WAIT portMAX_DELAY

typedef struct Cursor Cursor;

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
 * @brief Queues a cursor-position request.
 *
 * Moves the LCD cursor to the specified row and column. The actual cursor
 * movement is performed asynchronously by DISPLAY_Manager().
 *
 * @param row Target LCD row.
 * @param col Target LCD column.
 *
 * @return SUCCESS if the cursor request was queued successfully.
 * @return ERROR if the coordinates are invalid or the request could not be
 *         queued.
 */
base_t DISPLAY_SetCursor(uint8_t row, uint8_t col);

/**
 * @brief Queues a cursor-position request using a Cursor object.
 *
 * Moves the LCD cursor to the specified row(y) and column(x) defined by Cursor. The actual cursor
 * movement is performed asynchronously by DISPLAY_Manager().
 *
 * @param c Cursor struct
 *
 * @return SUCCESS if the cursor request was queued successfully.
 * @return ERROR if @p c is NULL or the request could not be queued.
 */
base_t DISPLAY_CursorSet(Cursor c);

/**
 * @brief Queues a string write request.
 *
 * Sends a null-terminated string to the display manager. The string is written
 * asynchronously by DISPLAY_Manager().
 *
 * The string contents must be copied internally before being queued. This
 * allows callers to pass stack-allocated strings safely.
 *
 * @param str Null-terminated string to display.
 *
 * @return SUCCESS if the string was queued successfully.
 * @return ERROR if @p str is NULL or the request could not be queued.
 */
base_t DISPLAY_Write(char *str);

/**
 * @brief Queues a display clear request.
 *
 * Requests the display manager to clear the LCD. The clear operation is
 * performed asynchronously by DISPLAY_Manager().
 *
 * @return SUCCESS if the clear request was queued successfully.
 * @return ERROR if the request could not be queued.
 */
base_t DISPLAY_Clear(void);

/**
 * @brief Main display processing loop.
 *
 * This function is intended to run inside a dedicated FreeRTOS task. It
 * continuously receives DISPLAY_Item messages from the internal queue and
 * executes the corresponding LCD operation.
 *
 * Normally does not return.
 *
 * @param args Unused FreeRTOS task parameter.
 */
void DISPLAY_Manager(void* args);

/**
 * @brief Starts the display manager task.
 *
 * Creates the FreeRTOS task that runs DISPLAY_Manager(). DISPLAY_Init() must
 * be called successfully before starting the task.
 *
 * @return SUCCESS if the task was created successfully.
 * @return ERROR if task creation failed.
 */
base_t DISPLAY_TaskStart();

#endif /* DISPLAY_H_ */

/** @} */

/** @} */
