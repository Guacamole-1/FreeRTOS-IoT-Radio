/**
* @file delay.c
* @brief This source file implements the API for creating delays
* @date 7 Oct 2025
* @author Grupo 6
*/

#include "Delay.h"
#include <math.h>
#include <stdbool.h>

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
#define TIMER2_PCLK_HZ   (SystemCoreClock / 4U)
#define TIMER2_US_PR     ((TIMER2_PCLK_HZ / 1000000U) - 1U)

#define ENABLE(BIT) 1 << BIT

#ifdef FREE_RTOS

// stuff to be filled in by the linker
#include "FreeRTOSConfig.h"
#include <stdint.h>

/* -------------------------------------------------------------------------
 * FREERTOS types
 * ------------------------------------------------------------------------- */

typedef void * QueueHandle_t;
typedef QueueHandle_t SemaphoreHandle_t;

typedef int32_t BaseType_t;
typedef uint32_t UBaseType_t;

#if ( configTICK_TYPE_WIDTH_IN_BITS == TICK_TYPE_WIDTH_16_BITS )
    typedef uint16_t TickType_t;
    #define portMAX_DELAY    ( ( TickType_t ) 0xffffU )
#elif ( configTICK_TYPE_WIDTH_IN_BITS == TICK_TYPE_WIDTH_32_BITS )
    typedef uint32_t TickType_t;
    #define portMAX_DELAY    ( ( TickType_t ) 0xffffffffUL )
#else
    typedef uint32_t TickType_t;
    #define portMAX_DELAY    ( ( TickType_t ) 0xffffffffUL )
#endif

#ifndef configTICK_RATE_HZ
#define configTICK_RATE_HZ     ( 1000U )
#endif

#define portTICK_PERIOD_MS     ( ( TickType_t ) 1000U / ( TickType_t ) configTICK_RATE_HZ )

#define pdTRUE                 ( ( BaseType_t ) 1 )
#define pdFALSE                ( ( BaseType_t ) 0 )

#define pdMS_TO_TICKS( xTimeInMs ) \
    ( ( TickType_t ) ( ( ( uint64_t ) ( xTimeInMs ) * ( uint64_t ) configTICK_RATE_HZ ) / ( uint64_t ) 1000U ) )

/* -------------------------------------------------------------------------
 *  semaphores macros
 * ------------------------------------------------------------------------- */

#define queueSEND_TO_BACK          ( ( BaseType_t ) 0 )
#define queueQUEUE_TYPE_MUTEX      ( ( uint8_t ) 1U )
#define semGIVE_BLOCK_TIME         ( ( TickType_t ) 0U )

QueueHandle_t xQueueCreateMutex( const uint8_t ucQueueType );
BaseType_t xQueueSemaphoreTake( QueueHandle_t xQueue, TickType_t xTicksToWait );
BaseType_t xQueueGenericSend( QueueHandle_t xQueue,
                              const void * const pvItemToQueue,
                              TickType_t xTicksToWait,
                              const BaseType_t xCopyPosition );

TickType_t xTaskGetTickCount( void );
void vTaskDelay( TickType_t xTicksToDelay );


#define xSemaphoreCreateMutex() \
    xQueueCreateMutex( queueQUEUE_TYPE_MUTEX )

#define xSemaphoreTake( xSemaphore, xBlockTime ) \
    xQueueSemaphoreTake( ( QueueHandle_t ) ( xSemaphore ), ( xBlockTime ) )

#define xSemaphoreGive( xSemaphore ) \
    xQueueGenericSend( ( QueueHandle_t ) ( xSemaphore ), NULL, semGIVE_BLOCK_TIME, queueSEND_TO_BACK )

static SemaphoreHandle_t delay_mutex = NULL;

#endif
// ---------
static volatile uint32_t tick;

static bool __init_flag = 0;

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
	if(__init_flag) return -1; // check if Init was already called
	__init_flag = 1;
	SC->PCONP |= ENABLE(PC_TIM2); // Timer 2 power/clock control bit.
	Timer2->TCR = ENABLE(CE_BIT) | ENABLE(CR_BIT) ; // Enable timer2 and reset
	Timer2->MCR = MR0_IRS; // enable interrupt on MR0 (MR0I), reset (MR0R) and stop TC and PC (MR0S)
	SC->PCLKSEL1 &= ~(0b11 << PCLK_TIMER2) ; // put PCLK at 00 (PCLK = CCLK/4) without afecting other clocks

	SystemCoreClockUpdate();
	Timer2->PR = TIMER2_US_PR;  // 1/PCLK*25 = 1µs   (has to be 24 because the 0 also counts)
#ifndef FREE_RTOS
	return (SysTick_Config(SYSTICK_FREQ) == 1) ? -1 : 0;
#else
	delay_mutex = xSemaphoreCreateMutex();
  return 0;
#endif
}
void DELAY_Milliseconds(uint32_t millis) {
#ifndef FREE_RTOS
	if(!__init_flag) return;
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
	if(!__init_flag) return 0;
	return (Get_Tick() - start_tick);
#else
  return (Get_Tick() - start_tick) * portTICK_PERIOD_MS;
#endif
}


void DELAY_Microseconds(uint32_t waitUs) {
/* #ifdef FREE_RTOS */
/*     if (delay_mutex == NULL) { */
/*         return; */
/*     } */

/*     if (xSemaphoreTake(delay_mutex, pdMS_TO_TICKS(1)) != pdTRUE) { */
/*         return; */
/*     } */
/* #endif */
	if(!__init_flag) return;
	Timer2->TCR = ENABLE(CR_BIT);
	Timer2->MR0 = waitUs;
	Timer2->IR = MR0_I_MASK;
	Timer2->TCR = ENABLE(CE_BIT); // enable timer and removes the timer reset
	uint32_t start_ms = Get_Tick();
	while((Timer2->IR & MR0_I_MASK) != 1){
		//__NOP();
		if (DELAY_GetElapsedMillis(start_ms) > 1) {
            break;
        }
	}
	Timer2->IR = MR0_I_MASK; // clear interrupt

}
