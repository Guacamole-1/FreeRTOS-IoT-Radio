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
// how do i make it wait for receive?
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
      default:
        break;
    }
  }
}
