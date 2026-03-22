/**
* @file delay.c
* @brief This source file implements the API for creating delays
* @date 7 Oct 2025
* @author Grupo 6
*/

#include "Delay.h"
#include <math.h>


//velocidade do cpu = 100 MHz
#define SYSTICK_FREQ (SystemCoreClock / 1000)
#define Timer2 LPC_TIM2
#define SC LPC_SC   	// system control
#define PC_TIM2 22 		//Timer 2 power/clock control bit.
#define PCLK_TIMER2 12 	// Peripheral clock selection for TIMER2.
#define CE_BIT 0 			// Counter Enable
#define CR_BIT 1			// Counter Reset
#define MR0_IRS 0b111		// MR0 (MR0I), reset (MR0R) and stop TC and PC (MR0S)
#define MR0_I_MASK 1				// MR0 Interrrupt
//formula for the PR so that every increment of TC is 1 µs
#define PR_1u (SystemCoreClock/4)*pow(10,-6) - 1

#define ENABLE(BIT) 1 << BIT

static volatile uint32_t tick;

#ifndef FREE_RTOS
void SysTick_Handler(void) {
	tick++;
}

uint32_t Get_Tick(){
	return tick;
}
#else
uint32_t Get_Tick(){
	return xTaskGetTickCount();
}
#endif


int DELAY_Init(void) {
	SC->PCONP |= ENABLE(PC_TIM2); // Timer 2 power/clock control bit.
	Timer2->TCR = ENABLE(CE_BIT) | ENABLE(CR_BIT) ; // Enable timer2 and reset
	Timer2->MCR = MR0_IRS; // enable interrupt on MR0 (MR0I), reset (MR0R) and stop TC and PC (MR0S)
	SC->PCLKSEL1 &= ~(0b11 << PCLK_TIMER2) ; // put PCLK at 00 (PCLK = CCLK/4) without afecting other clocks
	Timer2->PR = PR_1u;  // 1/PCLK*25 = 1µs   (has to be 24 because the 0 also counts)

	SystemCoreClockUpdate();
#ifndef FREE_RTOS
	return (SysTick_Config(SYSTICK_FREQ) == 1) ? -1 : 0;
#else
  return 0;
#endif
}
void DELAY_Milliseconds(uint32_t millis) {
#ifndef FREE_RTOS
	uint32_t start = tick;
	while ((tick - start) < millis) {
		__WFI();
	}
#else
  vTaskDelay(pdMS_TO_TICKS(millis));
#endif
}

uint32_t DELAY_GetElapsedMillis(uint32_t start_tick) {
#ifndef FREE_RTOS
	return (Get_Tick() - start_tick);
#else
  return (Get_Tick() - start_tick) * portTICK_PERIOD_MS;
#endif
}


void DELAY_Microseconds(uint32_t waitUs) {
	Timer2->TCR = ENABLE(CR_BIT);
	Timer2->MR0 = waitUs;
	Timer2->TCR = ENABLE(CE_BIT); // enable timer and removes the timer reset
	while((Timer2->IR & MR0_I_MASK) != 1){
		__NOP();
	}
	Timer2->IR = MR0_I_MASK; // clear interrupt
}
