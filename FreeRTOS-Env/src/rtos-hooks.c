/**
 * @file    rtos_hooks.c
 * @brief   FreeRTOS application hook functions.
 *
 * This module implements the hook functions required by FreeRTOS
 * to handle critical system errors such as stack overflow,
 * dynamic memory allocation failure and assertion failures.
 *
 * These hooks are called automatically by the kernel when the
 * corresponding protection mechanisms are enabled in FreeRTOSConfig.h.
 *
 * @version 1.0
 * @date    1 Feb 2026
 * @author  psampaio
 *
 * @note    This software is provided for educational purposes.
 * @warning Provided "AS IS", without any warranties.
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

/*
 * Called by FreeRTOS when a task overflows its stack.
 * Requires configCHECK_FOR_STACK_OVERFLOW > 0
 */
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pxTask;

    printf("\n*** STACK OVERFLOW DETECTED ***\n");
    printf("Task: %s\n", pcTaskName);

    taskDISABLE_INTERRUPTS();
    for (;;);
}

/*
 * Called when pvPortMalloc() fails to allocate memory.
 * Requires configUSE_MALLOC_FAILED_HOOK = 1
 */
void vApplicationMallocFailedHook(void)
{
    printf("\n*** MEMORY ALLOCATION FAILED ***\n");

    taskDISABLE_INTERRUPTS();
    for (;;);
}

/*
 * Called when a FreeRTOS assertion fails.
 * Triggered via configASSERT().
 */
void vAssertCalled(const char *file, int line)
{
    const char *name = pcTaskGetName(NULL);

    printf("\n*** FreeRTOS ASSERT FAILED ***\n");
    printf("Task: %s\n", name ? name : "N/A");
    printf("File: %s\n", file);
    printf("Line: %d\n", line);

    taskDISABLE_INTERRUPTS();
    for (;;);
}
