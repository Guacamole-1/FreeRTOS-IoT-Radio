/**
* @file LCD.h
* @brief This header file provides the API for controlling the LCD
* @version 2
* @date 26 Out 2025
* @author Grupo06
*
*/

/**
* @addtogroup DRIVERS
* @{
*/

/**
* @defgroup LCD LCD functions
* @brief This module provides core functions for the LCD HD44780 controller
* @{
*/

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
//#include <core_cm3.h>
#include <stdbool.h>
#include "LPC17xx.h"

//helper function to count args
#define _COUNT_ARGS( _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16, N, ...) N

// counts args from 0 to 16
#define COUNT_ARGS(...) \
    _COUNT_ARGS(, ##__VA_ARGS__, \
        16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#define MAX_CHARS    16*2                   // max amout of chars that can be displayed
#define MAX_ROWS     1
#define MAX_COLUMNS  15                  // MAX_ROWS and COLUMNS both include 0

#define TOP_4(x) (x & 0xF0) >> 4         // gets top 4 bits of a variable
#define BOTTOM_4(x) x & 0xF              // gets bottom 4 bits of a variable
#define GPIO2 LPC_GPIO2
#define SC LPC_SC                        // system control
#define PC_GPIO 15
#define LCD_BITS           0b1111111     // 7 bits for lcd
#define FUNCTION_SET       0b11
#define FUNCTION_SET2      0b10
#define FUNCTION_SET3      (lcd16_t) {.DB=0b00101000}    // 5x8 character font and two line display

#define DISPLAY_OFF        (lcd16_t) {.DB=0b1000}
#define DISPLAY_ON         (lcd16_t) {.DB=0b1100}
#define DISPLAY_CLEAR      (lcd16_t) {.DB=0b1}
#define ENTRY_MODE         (lcd16_t) {.DB=0b110}

#define CURSOR_SETUP(x)    (lcd16_t) {.DB=(0b1100 | (x))}
#define CURSOR_ON          1 << 1
#define CURSOR_OFF         0
#define BLINK_ON           1
#define BLINK_OFF          0



#define ROW_OFFSET         0x40
#define SET_DDRAM_ADDR(x)  (1 << 7) | x
#define ARRAY_SIZE(x) sizeof(x) / sizeof(x[0])

// (x,y)  ->  (columns,rows)
typedef struct Cursor {
    uint8_t x;
    uint8_t y;
} Cursor;

/**
* @brief sets cursor to specified row and column
* rows    = 0-1
* columns = 0-15
* @returns void
*/
void LCDText_SetCursor(int row, int column);

/**
* @brief sets cursor to specified row and column using Cursor type
* @returns void
*/
void LCDText_CursorSet(Cursor c);

#if defined(__GNUC__)
  typedef union
  {
    struct
    {
      uint8_t RS  : 1; // LSB
      uint8_t E   : 1;
      uint8_t DB 	: 4; // MSB
    };
    struct
    {
      uint8_t data : 7;
    };

  } lcd_t ; // P2[0-6]

  typedef union
  {
    struct
    {
      uint8_t RS  : 1;  // LSB
      uint8_t E   : 1;
      uint8_t DB  : 8;  // MSB
    };
    struct
    {
      uint16_t data : 11;
    };
  } lcd16_t ; // P2[0-6]
#else
#error "Error: Bit fields not supported"
#endif

/**
* @brief gives column position for cursor to be centered
* @returns value to put cursor in
*/
int LCDText_Center(const char *text);


/**
 * @brief writes to the LCD using a lcd_t type (DB = 4 bits)
 * @returns void
 */
void lcd_write(lcd_t* LCD);
/**
 * @brief writes to the LCD using a lcd16_t type (DB = 8 bits)
 * @returns void
 */
void lcd_write8(lcd16_t* LCD16);
/**
 * @brief Initializes LCD screen
 * @warning DELAY_Init() MUST be called before using this function!
 * @note This function MUST be called before using any LCD functions
 * @returns void
 */
void LCDText_Init(void);
/**
 * @brief writes a single char in the current cursor position
 * @returns void
 */
void LCDText_WriteChar(char ch);

/**
 * @brief writes a string with text wrapping in the first line
 * second line will write to LCD RAM if str is bigger than the line length
 * @returns void
 */
void LCDText_WriteString(char *str);

/**
 * @brief Clears display and sets the cursor to position 0,0
 * @returns void
 */
void LCDText_Clear(void);

/**
 * @brief a printf type macro to print to the LCD
 * @returns void
 */
#define LCDText_Printf(fmt, ...)                              \
  __LCDText_Printf(fmt, COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)

/**
 * @brief  a printf type function to print to the LCD (use LCDText_Printf() macro)
 * @note  Use the LCDText_Printf() macro to not have the count argument
 * @returns void
 */
void __LCDText_Printf(char *fmt, int count, ...);

#endif /* LCD_H_ */

/**
 * @}
 */

/**
 * @}
 */

