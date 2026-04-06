
/**
 * @file base.h
 * @brief This header file provides base_t enum for error flags
 * @version 1
 * @date 4 Apr 2026
 * @author Grupo08
 *
 */

#ifndef BASE_H
#define BASE_H

typedef enum {
	NOT_INITIALIZED = -1,
	SUCCESS,
	ALREADY_INITIALIZED,
	ERROR,
	LOCK_ERROR,
	ARG_ERROR,
	QUEUE_FULL,
	QUEUE_EMPTY,
	ERROR_OUT_OF_MEMORY,
} base_t ;

#endif
