/**
 * @file task_app.h
 * @brief This header file provides the API for starting the main application task.
 * @version 1
 * @date 12 Jun 2026
 * @author Grupo 08
 *
 */

/**
 * @defgroup task_app_module Application Task Module
 * @ingroup free_rtos
 * @brief Main FreeRTOS application task launcher.
 * @{
 */

#ifndef TASK_APP_H_
#define TASK_APP_H_

#include <stdint.h>

/**
 * @brief Creates the main application FreeRTOS task.
 *
 * This function creates the task responsible for initializing the application
 * modules and starting the main menu flow. It does not start the scheduler.
 *
 * @return 0 if the task was created successfully.
 * @return -1 if task creation failed.
 */
int32_t App(void);

#endif /* TASK_APP_H_ */

/** @} */
