/**
 * @file menu_rtos.h
 * @brief This header file provides the FreeRTOS-specific menu API.
 * @version 1
 * @date 12 Jun 2026
 * @author Grupo 08
 *
 */

/**
 * @defgroup menu_rtos_module Menu RTOS Module
 * @ingroup free_rtos
 * @brief FreeRTOS integration layer for the application maintenance menu.
 * @{
 */

#ifndef MENU_RTOS_H_
#define MENU_RTOS_H_

#include "base.h"
#include "Menu.h"

/**
 * @brief Opens the maintenance menu.
 *
 * Displays the maintenance options used to edit calendar/clock values, delete
 * stored radio data, synchronize the clock through NTP and connect to Wi-Fi.
 *
 * This function is called from the main menu in Menu.h, it
 * this overrides the original maintenance menu 
 */
void Menu_Maintenance(void);

#endif /* MENU_RTOS_H_ */

/** @} */
