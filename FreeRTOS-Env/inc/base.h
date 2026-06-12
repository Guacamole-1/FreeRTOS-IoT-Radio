/**
 * @file base.h
 * @brief This header file provides the common return-code type used by the project.
 * @version 1
 * @date 4 Apr 2026
 * @author Grupo08
 *
 */

/**
 * @defgroup base_module Base Module
 * @ingroup free_rtos
 * @brief Common status and error codes shared by the FreeRTOS modules.
 * @{
 */

#ifndef BASE_H
#define BASE_H

/**
 * @brief Generic return codes used by project modules.
 *
 * These values are used by RTOS wrappers and application modules to report
 * success, initialization state, locking errors, argument errors, queue errors
 * and memory allocation failures.
 */
typedef enum {
    /** Module or subsystem was used before initialization. */
    NOT_INITIALIZED = -1,

    /** Operation completed successfully. */
    SUCCESS,

    /** Initialization was requested, but the module was already initialized. */
    ALREADY_INITIALIZED,

    /** Generic operation error. */
    ERROR,

    /** Internal lock could not be created, taken or released. */
    LOCK_ERROR,

    /** Invalid argument supplied by the caller. */
    ARG_ERROR,

    /** Queue send operation failed because the queue was full. */
    QUEUE_FULL,

    /** Queue receive operation failed because the queue was empty. */
    QUEUE_EMPTY,

    /** Dynamic memory allocation failed. */
    ERROR_OUT_OF_MEMORY,
} base_t;

#endif /* BASE_H */

/** @} */
