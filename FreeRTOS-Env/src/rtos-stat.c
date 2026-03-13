/**
 * @file    rtos_stat.c
 * @brief   FreeRTOS runtime statistics timer configuration.
 *
 * This module configures TIMER1 of the LPC1769 to be used as the time base
 * for FreeRTOS runtime statistics. It allows the operating system to measure
 * how much CPU time each task consumes.
 *
 * The timer runs as a free-running counter with a fixed time resolution,
 * providing a reference clock used by the kernel when
 * `configGENERATE_RUN_TIME_STATS` is enabled.
 *
 * @version 1.0
 * @date    12 Jan 2026
 * @author  psampaio
 *
 * @note    This software is provided for educational purposes.
 * @warning Provided "AS IS", without any warranties.
 *
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>

/* Timer Control Register bits */
#define TCR_COUNT_RESET     2   // Reset the timer counter
#define TCR_COUNT_ENABLE    1   // Enable the timer

/* Counter/Timer Control Register mode */
#define CTCR_CTM_TIMER      0x00  // Timer mode (counts clock pulses)

/*
 * Configures TIMER1 to be used as the time base for FreeRTOS runtime stats.
 * This function is automatically called by FreeRTOS when:
 * configGENERATE_RUN_TIME_STATS = 1
 */
void TIMER_RuntimeStatsConfig(void)
{
    /* Enable power to TIMER1 */
    LPC_SC->PCONP |= (1 << 2);

    /* Set peripheral clock for TIMER1 = CCLK */
    LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(0x3 << 4))) | (0x01 << 4);

    /* Reset timer */
    LPC_TIM1->TCR = TCR_COUNT_RESET;

    /* Use timer mode (increment every clock tick after prescaler) */
    LPC_TIM1->CTCR = CTCR_CTM_TIMER;

    /* Prescaler: increments every 100 µs */
    LPC_TIM1->PR = (SystemCoreClock / 10000UL) - 1UL;

    /* Start timer */
    LPC_TIM1->TCR = TCR_COUNT_ENABLE;
}

/*
 * Returns the current TIMER1 counter value.
 * FreeRTOS uses this to compute CPU usage per task.
 */
uint32_t TIMER_RuntimeStatsGet(void)
{
    return LPC_TIM1->TC;
}
