/*
 * mutex_wrapper.h
 *
 *  Created on: Mar 29, 2026
 *      Author: guack
 */

#ifndef MUTEX_WRAPPER_H_
#define MUTEX_WRAPPER_H_

#define _MUTEX_BLOCK_TIME portMAX_DELAY

#define LOCK_DEF static SemaphoreHandle_t __lock_semaphore		\
  static bool __init_flag

#define LOCK_INIT __lock_semaphore = xSemaphoreCreateBinary(); \
  if (__lock_semaphore == NULL ) return -1;					   \
  else __init_flag = 1;										   \

#define LOCK                                                      \
  if ( (__init_flag) && xSemaphoreTake(__lock_semaphore, _MUTEX_BLOCK_TIME) == pdTRUE) {

#define UNLOCK xSemaphoreGive(__lock_semaphore);		\
  } else {return -1;} \


#endif /* MUTEX_WRAPPER_H_ */
