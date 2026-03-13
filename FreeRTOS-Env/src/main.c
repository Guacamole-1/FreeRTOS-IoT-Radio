/*
 * main.c
 *
 *  Created on: Feb 12, 2026
 *      Author: psampaio
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "task_app.h"

int main(void) {
    printf("FreeRTOS on LPC1769 @ %ld MHz\n", SystemCoreClock / 1000000UL);

    if (App() != 0) {
        printf("APP init failed!\n");
        while (1);
    }

    vTaskStartScheduler();

    printf("Scheduler failed to start! Check heap size.\n");
    while (1);

    return 0;
}
