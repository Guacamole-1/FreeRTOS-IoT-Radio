/**
* @file delay.h
* @brief This header file provides the API for creating delays
* @date 7 Oct 2025
* @author Grupo 6
*/
/**
* @addtogroup DRIVERS
* @{
*/
/**
* @defgroup Delay Delay functions
* @brief This module provides core functions for creating time delays
* in the program, allowing precise waits in both milliseconds
* and microseconds.
* @{
*/
#ifndef DELAY_H_
#define DELAY_H_

//#ifdef __USE_CMSIS
#include "LPC17xx.h"
//#endif

#include <stdio.h>
/**
* @brief handles when systick generates a interrupt every millisecond
* and increments tick
* @note This function is called "automatically"
*/
void SysTick_Handler(void);

/**
* @brief returns tick variable
* @return uint32_t
*/
uint32_t Get_Tick();
/**
* @brief Enables timer2 and systick
* @return 0 if initialization is successful; -1 if initialization fails.
* @note This function MUST be called before using any DELAY functions
*/
int DELAY_Init(void);
/**
* @brief Delays millis amount of milliseconds (blocking)
* @param millis amount of milliseconds to delay
*/
void DELAY_Milliseconds(uint32_t millis);
/**
* @brief Get how many milliseconds have passed since start
* using systick
* @return uint32_t milliseconds since start
*/
uint32_t DELAY_GetElapsedMillis(uint32_t start);

/**
* @brief Delays waitUs microseconds (blocking) using timer2
* @param waitUs microseconds to wait for
*/
void DELAY_Microseconds(uint32_t waitUs);

#endif /* DELAY_H_ */

/**
* @}
*/
/**
* @}
*/

