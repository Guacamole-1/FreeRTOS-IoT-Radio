/**
 * @file button.h
 * @brief This header file provides the API for controlling the NAV7BTN with concurrent access
 * @version 1
 * @date 11 Mar 2026
 * @author Grupo 08
 *
 */

/**
 * @defgroup button_module Button Module
 * @ingroup free_rtos
 * @brief RTOS wrapper for NAV7BTN buttons
 * @{
 */
#ifndef BUTTON_H
#define BUTTON_H

#include "Nav7Btn.h"
#include "base.h"
#define BUTTON_TICKS_TO_WAIT portMAX_DELAY


/**
 * @brief Initializes the NAV button subsystem.
 *
 * Initializes:
 * - NAV7BTN
 * - internal button queue
 * - xTimer
 *
 * Must be called before any other BUTTON function.
 *
 * @return SUCCESS on success
 * @return ALREADY_INITIALIZED if it was already called once
 * @return ERROR on error
 */
base_t BUTTON_Init(void);
/**
 * @brief Gets the pressed button between two "STN_SCAN_PERIOD"'s recorded
 * by a timer initialized in BUTTON_Init
 *
 * @param pressed variable to store the pressed value
 * @return NOT_INITIALIZED when BUTTON_Init wasnt called
 * @return SUCESS on sucess
 */
base_t BUTTON_Pressed(NAVBTN_TypeDef* pressed);
#endif

/** @} */
