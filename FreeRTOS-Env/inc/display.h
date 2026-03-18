/*
 * display.h
 *
 *  Created on: Mar 11, 2026
 *      Author: guack
 */
// install doxygen!!
#ifndef DISPLAY_H_
#define DISPLAY_H_

#define DISPLAY_MAX_QUEUE 10
#define DISPLAY_TICKS_TO_WAIT pdMS_TO_TICKS(100)
typedef enum {
	DISP_SUCCESS,
	DISP_INIT_ERROR,
	DISP_QUEUE_ERROR,
	DISP_QUEUE_FULL,
  DISP_QUEUE_EMPTY
} DISPLAY_STATUS;

typedef enum {
	WRITE_STR,
	WRITE_LINE,
	CLEAR,
	LOCATE,
	CURSOR_ON,
	CURSOR_OFF,
  KILL
} DISPLAY_type;

typedef struct {
	DISPLAY_type id;
	void* args;
} DISPLAY_Item;

typedef struct {
	const char* str;
} DISPLAY_Args_WS;

typedef struct {
  const char* str1;
  const char* str2;
} DISPLAY_Args_WL;

typedef struct {
  int line;
  int column;
} DISPLAY_Args_L;

DISPLAY_STATUS DISPLAY_Init();
DISPLAY_STATUS DISPLAY_Send(DISPLAY_Item* item);
DISPLAY_STATUS DISPLAY_Receive(DISPLAY_Item* recvd_item);
DISPLAY_STATUS DISPLAY_Manager();

#define DISPLAY_Printf(fmt, ...)                              \
  __DISPLAY_Printf(fmt, COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)


#endif /* DISPLAY_H_ */
