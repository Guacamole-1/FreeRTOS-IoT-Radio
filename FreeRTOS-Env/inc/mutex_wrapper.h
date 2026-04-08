/**
 * @file mutex_wrapper.h
 * @brief This header file provides macros for the facilitation of creation and use of mutexes
 * @version 1
 * @date 29 Mar 2026
 * @author Grupo08
 *
 */


#ifndef MUTEX_WRAPPER_H_
#define MUTEX_WRAPPER_H_
#include "base.h"
#include <stdbool.h>
#define _MUTEX_BLOCK_TIME portMAX_DELAY

#define INIT_FLAG		__init_flag
#define INIT_FLAG_DEF	static bool INIT_FLAG;
#define SET_INIT_FLAG	INIT_FLAG = true;

#define LOCK_DEF static SemaphoreHandle_t __lock_semaphore;		\
  static bool INIT_FLAG;

#define LOCK_INIT __lock_semaphore = xSemaphoreCreateMutex();	\
	configASSERT(__lock_semaphore != NULL);						\
	INIT_FLAG = 1;												\

#define LOCK                                                      \
  if ( (INIT_FLAG) && xSemaphoreTake(__lock_semaphore, _MUTEX_BLOCK_TIME) == pdTRUE) {

#define UNLOCK xSemaphoreGive(__lock_semaphore);		\
	} else {return NOT_INITIALIZED;} \

//check if LOCK_INIT was ran, and if not return -1
#define INIT_CHECK	if(!INIT_FLAG) return NOT_INITIALIZED;
//return -1 if LOCK_INIT was already ran, (opposite of INIT_CHECK)
#define INIT_NCHECK	if(INIT_FLAG) return ALREADY_INITIALIZED;
#endif /* MUTEX_WRAPPER_H_ */
