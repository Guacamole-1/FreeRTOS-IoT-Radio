/**
* @file Nav7Btn.h
* @brief This header file provides the API for controlling the Nav7Btn keypad
* @version 1.1
* @date 27 Oct 2025
* @author Grupo06
*
*/
/**
* @addtogroup DRIVERS
* @{
*/

/**
* @defgroup Nav7Btn Nav7Btn functions
* @brief This module provides core functions for Nav7Btn keypad
* @{
*/

#ifndef NAV7BTN_H_
#define NAV7BTN_H_

#include <stdint.h>
#define GPIO0 LPC_GPIO0
#define SC LPC_SC// system control
#define PC_GPIO 15
//#define COL_BITS 0b11
//#define ROW_BITS (0b1111 << 6)
#define NAV_BITS (COL_BITS | ROW_BITS)
#define BIT_POS(x) 1<<x

#define COL0_PIN 15
#define COL1_PIN 16
#define ROW0_PIN 23
#define ROW1_PIN 24
#define ROW2_PIN 25
#define ROW3_PIN 26

#define COL_BITS ((1U << COL0_PIN) | (1U << COL1_PIN))
#define ROW_BITS ((1U << ROW0_PIN) | (1U << ROW1_PIN) | (1U << ROW2_PIN) | (1U << ROW3_PIN))


typedef enum {
NAVBTN_NONE = -1,
NAVBTN_UP = 1, NAVBTN_DOWN,
NAVBTN_LEFT, NAVBTN_RIGHT, NAVBTN_CENTER,
NAVBTN_BACK, NAVBTN_ENTER
} NAVBTN_TypeDef;

#if defined(__GNUC__)
typedef union
{
    struct
    {
        uint8_t columns  : 2; // LSB
        uint8_t rows     : 4; // MSB
    };
    struct
    {
        uint8_t data ;
    };

} Nav7Btn;
#else
#error "Error: Bit fields not supported"
#endif

/**
 * @brief Enable the peripheral Nav7Btn, by initializing the GPIO, granting acess
 * to the keypad
 */
void NAVBTN_Init(void);

/**
 * @brief Read the input from the peripheral, by checking each row of each collumn, and checking
 * if there is an output
 * @return -1 if no button was pressed or the constant associated with the key(NAVBTN_UP = 1, NAVBTN_DOWN = 2)
 */
NAVBTN_TypeDef NAVBTN_Read(void);

/**
 * @brief Returns the button that was read between two consecutive calls of the function
 * @return if true returns pressed button, if false returns -1
 */
NAVBTN_TypeDef NAVBTN_Pressed(void);

/**
 * @brief associates the enum list with the correspondent char
 * @return the correspondent char, ex: NAVBTN_UP = 1 returns "UP"
 */
char* NAVBTN_GetName(NAVBTN_TypeDef nav);

#endif /* NAV7BTN_H_ */

/**
* @}
*/

/**
* @}
*/
