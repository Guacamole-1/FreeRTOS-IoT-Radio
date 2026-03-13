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



#endif /* DISPLAY_H_ */
