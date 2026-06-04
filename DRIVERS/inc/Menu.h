/**
* @file Menu.h
* @brief This header file provides the API for the Menu functions
* @version 1.1
* @date 9 Nov 2025
* @author Grupo06
*
*/
/**
* @addtogroup DRIVERS
* @{
*/

/**
* @defgroup Menu Menu functions
* @brief This module provides core functions for Menu
* @{
*/

#ifndef MENU_H_
#define MENU_H_

#include "Fields/Field.h"
#include "Fields/Spinner.h"
#include "Radio.h"

typedef void (* func)(void* arg);
/**
 * @brief calendar Menu to edit the date
 * 
 */
void Calendar_Menu();
/**
 * @brief Clock Menu to edit the time
 * 
 */
void Clock_Menu();
/**
 * @brief maintence menu to select available functions
 * 
 */
void Menu_Maintenance();
/**
 * @brief Menu State Machine, A generic state machine that executes field's functions  
 * 
 * @param always_render always render at each loop
 * @param main_state if it is a root state (cannot exit this state)
 * @return true on save, false on cancel
 */ 
bool Menu_SM(Field* field,bool always_render,bool main_state);
/**
 * @brief Initializes Menu (NavBTN,DELAY,LCDText,RTC)
 * @param Rdata data to Initialize the Radio
 */
void Menu_Init(Radio_Data* Rdata);
/**
 * @brief Main menu where you see time and date, frequency and volume
 * and increment/decrement time and volume
 * 
 */
void Main_Menu();
/**
 * @brief Generic Confirmation menu
 * 
 * @param Conf_text Confirmation text (ex: "Are you sure...")
 * @param yes function to execute if yes
 * @param no function to execute if no
 */
void Confirmation_Menu(char* Conf_text, action yes, action no);


#endif /* MENU_H_ */

/**
* @}
*/

/**
* @}
*/

