/*
 * display.c
 *
 *  Created on: Mar 11, 2026
 *      Author: guack
 */

#include "FreeRTOS.h"

/* Kernel includes. */
#include "task.h"
#include "queue.h"

#include "display.h"
#include "delay.h"
#include "lcdtext.h"
#include <stdarg.h>
#include <stdio.h>

#define MAX_CHARS 16*2

//helper function to count args
#define _COUNT_ARGS( _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16, N, ...) N

// counts args from 0 to 16
#define COUNT_ARGS(...)                                 \
  _COUNT_ARGS(, ##__VA_ARGS__,                          \
              16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)


static QueueHandle_t disp_queue;

DISPLAY_STATUS DISPLAY_Init(){
	if(!DELAY_Init()){
		return DISP_INIT_ERROR;
	}
	disp_queue = xQueueCreate(DISPLAY_MAX_QUEUE,sizeof(DISPLAY_Item));
	if (disp_queue == NULL){
		return DISP_QUEUE_ERROR;
	}
	return DISP_SUCCESS;
}

DISPLAY_STATUS DISPLAY_Send(DISPLAY_Item* item){
	if(xQueueSend(disp_queue,item,DISPLAY_TICKS_TO_WAIT) == errQUEUE_FULL){
		return DISP_QUEUE_FULL;
	}
	return DISP_SUCCESS;
}
DISPLAY_STATUS DISPLAY_Receive(DISPLAY_Item* recvd_item){
  return xQueueReceive(disp_queue, (void*)recvd_item, DISPLAY_TICKS_TO_WAIT) == pdPASS ?
    DISP_SUCCESS : DISP_QUEUE_EMPTY;
}


void __DISPLAY_Printf(const char* fmt, int count, ...){
  char str[MAX_CHARS+1];
  va_list args;
  va_start(args, count);
  vsnprintf(str,MAX_CHARS,fmt,args);
  DISPLAY_Item item = {WRITE_STR,str};
  DISPLAY_Send(&item);
  va_end(args);
}

DISPLAY_STATUS DISPLAY_Manager() {
  DISPLAY_Item recvd_item = {0};
  while (1) {
    DISPLAY_Receive(&recvd_item);
    switch (recvd_item.id) {
      case WRITE_STR:
        LCDText_WriteString(((DISPLAY_Args_WS*)recvd_item.args)->str );
        break;
      case WRITE_LINE:
        {
          DISPLAY_Args_WL* args = (DISPLAY_Args_WL*)recvd_item.args;
          LCDText_WriteLine(args->str1, args->str2);
          break;
        }
      case CLEAR:
        LCDText_Clear();
        break;
      case LOCATE:
        {
          DISPLAY_Args_L* args = (DISPLAY_Args_L*)recvd_item.args;
          LCDText_Locate(args->line, args->column);
          break;
        }
      case CURSOR_ON:
        LCDText_CursorOn();
        break;
      case CURSOR_OFF:
        LCDText_CursorOff();
        break;
      case KILL:
        return DISP_SUCCESS;
      default:
        break;
    }
  }
}
