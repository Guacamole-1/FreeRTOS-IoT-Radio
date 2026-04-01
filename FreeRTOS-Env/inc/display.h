/**
 * @file display.h
 * @brief This header file provides the API for controlling the LCD Display with
 * concurrent access
 * @version 1
 * @date 11 Mar 2026
 * @author Grupo 08
 *
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#define DISPLAY_MAX_QUEUE 10
#define DISPLAY_TICKS_TO_WAIT portMAX_DELAY

typedef enum {
	DISP_SUCCESS,
	DISP_INIT_ERROR,
	DISP_QUEUE_ERROR,
	DISP_QUEUE_FULL,
  DISP_QUEUE_EMPTY
} DISPLAY_STATUS;

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



DISPLAY_STATUS DISPLAY_Init();
DISPLAY_STATUS DISPLAY_Send(DISPLAY_Item item);
DISPLAY_STATUS DISPLAY_Receive(DISPLAY_Item** recvd_item);
DISPLAY_STATUS DISPLAY_Manager();
void DISPLAY_Printf(const char* fmt,...);


#endif /* DISPLAY_H_ */
