/*
 * button.h
 *
 *  Created on: 01/04/2026
 *      Author: rafav
 */

#ifndef BUTTON_H
#define BUTTON_H

#include "Nav7Btn.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdbool.h>

#define BUTTON_TICKS_TO_WAIT portMAX_DELAY


typedef enum {
	BUTTON_NOT_INITIALIZED = -1,
	BUTTON_SUCCESS,
	BUTTON_INIT_FAIL,
} BUTTON_STATUS;

typedef BUTTON_TypeDef;

bool BUTTON_Init(void);

#endif

