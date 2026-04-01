/**
 * @file display.c
 * @brief This source file implements the API for controlling the LCD Display with
 * concurrent access
 * @version 1
 * @date 11 Mar 2026
 * @author Grupo 08
 *
 */

#include "FreeRTOS.h"

/* Kernel includes. */
#include "portable.h"
#include "projdefs.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "display.h"
#include "Delay.h"
#include "LCD.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 16*2



static QueueHandle_t disp_queue;


DISPLAY_STATUS DISPLAY_Init(){
	if(DELAY_Init()){
		return DISP_INIT_ERROR;
	}
	LCDText_Init();
	disp_queue = xQueueCreate(DISPLAY_MAX_QUEUE,sizeof(DISPLAY_Item*));
	if (disp_queue == NULL){
		return DISP_QUEUE_ERROR;
	}

	return DISP_SUCCESS;
}


void free_item(DISPLAY_Item* item){
  if (item == NULL) return;

  if (item->id == WRITE_STR && item->args != NULL)
  {
    vPortFree(((DISPLAY_Args_WS*)item->args)->str);
  }
  if(item->args != NULL){
    vPortFree(item->args);
  }
  vPortFree(item);
}

DISPLAY_Item* copy_item(DISPLAY_Item item){
  DISPLAY_Item* new_item = pvPortMalloc(sizeof(DISPLAY_Item));
  new_item->id = item.id;
  switch (new_item->id)
    {
      case WRITE_STR:
        {
          new_item->args = pvPortMalloc(sizeof(DISPLAY_Args_WS));
          memcpy(new_item->args, item.args, sizeof(DISPLAY_Args_WS));
          DISPLAY_Args_WS* old_args = (DISPLAY_Args_WS*)item.args;
          DISPLAY_Args_WS* new_args = (DISPLAY_Args_WS*)new_item->args;
          // copy string
          new_args->str = pvPortMalloc(strlen(old_args->str) + 1); // +1 for '\0'
          memcpy(new_args->str, old_args->str, strlen(old_args->str) + 1);
          break;
        }
        break;
      case CURSOR_SET:
        new_item->args = pvPortMalloc(sizeof(Cursor));
        memcpy(new_item->args, item.args, sizeof(Cursor));
        break;
      case WRITE_CMD:
        new_item->args = pvPortMalloc(sizeof(lcd16_t));
        memcpy(new_item->args, item.args, sizeof(lcd16_t));
        break;
      default:
        new_item->args = NULL;
        break;
    }
  return new_item;
}


DISPLAY_STATUS DISPLAY_Send(DISPLAY_Item item){
  DISPLAY_Item* new_item = copy_item(item);
	if(xQueueSend(disp_queue,&new_item,DISPLAY_TICKS_TO_WAIT) == errQUEUE_FULL){
    free_item(new_item);
		return DISP_QUEUE_FULL;
	}
	return DISP_SUCCESS;
}
DISPLAY_STATUS DISPLAY_Receive(DISPLAY_Item** recvd_item){
  return xQueueReceive(disp_queue, recvd_item, DISPLAY_TICKS_TO_WAIT) == pdPASS ?
    DISP_SUCCESS : DISP_QUEUE_EMPTY;
}


void DISPLAY_Printf(const char* fmt, ...){
  char str[MAX_CHARS+1];
  va_list args;

  va_start(args, fmt);
  vsnprintf(str,MAX_CHARS+1,fmt,args);
  va_end(args);

  DISPLAY_Args_WS str_args = {str};
  DISPLAY_Item item = {WRITE_STR,&str_args};
  DISPLAY_Send(item);
}

DISPLAY_STATUS DISPLAY_Manager() {
  while (1) {
    DISPLAY_Item* recvd_item = NULL;
    if (DISPLAY_Receive(&recvd_item) != DISP_QUEUE_EMPTY)
      {
        switch (recvd_item->id)
          {
          case WRITE_STR:
            LCDText_WriteString(((DISPLAY_Args_WS*)recvd_item->args)->str);
            break;
          case CLEAR:
            LCDText_Clear();
            break;
          case CURSOR_SET:
            {
              Cursor* args = (Cursor*)recvd_item->args;
              LCDText_CursorSet(*args);
              break;
            }
          case WRITE_CMD:
            lcd_write8((lcd16_t*)(recvd_item->args));
            break;
          default:
            break;
          }
        free_item(recvd_item);
      }
    else {
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }
}
