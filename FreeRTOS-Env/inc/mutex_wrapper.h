/**
 * @file mutex_wrapper.h
 * @brief This header file provides helper macros for module initialization and mutex usage.
 * @version 1
 * @date 29 Mar 2026
 * @author Grupo08
 *
 */

/**
 * @defgroup mutex_wrapper_module Mutex Wrapper Module
 * @ingroup free_rtos
 * @brief Macros used to create and use one mutex per RTOS wrapper module.
 * @{
 */

#ifndef MUTEX_WRAPPER_H_
#define MUTEX_WRAPPER_H_

#include "base.h"
#include <stdbool.h>

/**
 * @brief Blocking time used when taking a module mutex.
 */
#define _MUTEX_BLOCK_TIME portMAX_DELAY

/**
 * @brief Name of the internal initialization flag used by wrapper modules.
 */
#define INIT_FLAG        __init_flag

/**
 * @brief Defines the internal initialization flag for modules that do not use LOCK_DEF.
 */
#define INIT_FLAG_DEF    static bool INIT_FLAG;

/**
 * @brief Marks a module as initialized.
 */
#define SET_INIT_FLAG    INIT_FLAG = true;

/**
 * @brief Defines the internal mutex and initialization flag for a module.
 *
 * This macro should be placed once at file scope in the module implementation.
 */
#define LOCK_DEF static SemaphoreHandle_t __lock_semaphore;        \
  static bool INIT_FLAG;

/**
 * @brief Creates the internal module mutex and sets the initialization flag.
 *
 * Must be called during the module initialization function after any required
 * low-level initialization has succeeded.
 */
#define LOCK_INIT() __lock_semaphore = xSemaphoreCreateMutex();    \
    configASSERT(__lock_semaphore != NULL);                        \
    INIT_FLAG = 1                                                \

/**
 * @brief Starts a protected section by taking the internal module mutex.
 *
 * The macro opens a C block. It must be paired with UNLOCK(). If the module is
 * not initialized, the calling function returns NOT_INITIALIZED.
 */
#define LOCK()                                                      \
  if ( (INIT_FLAG) && xSemaphoreTake(__lock_semaphore, _MUTEX_BLOCK_TIME) == pdTRUE) {

/**
 * @brief Ends a protected section by releasing the internal module mutex.
 *
 * This macro closes the C block opened by LOCK().
 */
#define UNLOCK() xSemaphoreGive(__lock_semaphore);        \
    } else {return NOT_INITIALIZED;} \

/**
 * @brief Returns NOT_INITIALIZED if the module initialization flag is false.
 */
#define INIT_CHECK()    if(!INIT_FLAG) return NOT_INITIALIZED

/**
 * @brief Returns ALREADY_INITIALIZED if the module initialization flag is true.
 */
#define INIT_NCHECK()    if(INIT_FLAG) return ALREADY_INITIALIZED

#endif /* MUTEX_WRAPPER_H_ */

/** @} */
