/**
 * @file lcdtext.h
 * @brief Text LCD Driver API (HD44780, 4-bit mode).
 *
 * This module provides an interface to control a 4-bit HD44780-compatible
 * character LCD connected to the LPC1769 microcontroller.
 * It supports text output, cursor control and display configuration.
 *
 * @version 1.0
 * @date 30 Oct 2018
 * @author PSampaio
 *
 * @note This software is provided for educational purposes.
 * @warning Provided "AS IS", without any warranties.
 */

#ifndef LCDTEXT_H_
#define LCDTEXT_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup DRIVERS
 * @{
 */

/**
 * @defgroup LCDTEXT LCDTEXT — Text LCD Driver
 * @brief Driver for HD44780-compatible character LCD.
 *
 * The LCD operates in 4-bit mode.
 *
 * ### Hardware connections
 *
 * LCD Pin | LPC1769 Pin
 * ------- | -----------
 * D4–D7   | P2.0–P2.3
 * EN      | P0.11
 * RS      | P0.10
 * RW      | GND
 *
 * @{
 */

#define LCDTEXT_LINES   2   /**< Number of display lines */
#define LCDTEXT_COLUMNS 16  /**< Number of columns per line */

/**
 * @brief Initializes the LCD.
 *
 * Configures the LCD hardware and sets it to default display mode.
 *
 * @note Must be called before using any other LCDText functions.
 */
void LCDText_Init(void);

/**
 * @brief Writes a single character.
 *
 * @param c Character to display.
 */
void LCDText_WriteChar(char c);

/**
 * @brief Writes a null-terminated string.
 *
 * @param str String to display.
 */
void LCDText_WriteString(const char *str);

/**
 * @brief Writes text to both display lines.
 *
 * @param firstLine  Text for line 0 (NULL to leave unchanged)
 * @param secondLine Text for line 1 (NULL to leave unchanged)
 */
void LCDText_WriteLine(const char *firstLine, const char *secondLine);

/**
 * @brief Clears the display and moves the cursor to home (0,0).
 */
void LCDText_Clear(void);

/**
 * @brief Moves the cursor to a specific location.
 *
 * @param line   Line index (0–LCDTEXT_LINES-1)
 * @param column Column index (0–LCDTEXT_COLUMNS-1)
 */
void LCDText_Locate(int line, int column);

/** @brief Enables the visible cursor. */
void LCDText_CursorOn(void);

/** @brief Disables the visible cursor. */
void LCDText_CursorOff(void);

/**
 * @brief Creates a custom character.
 *
 * @param location CGRAM location (0–7)
 * @param charmap  Pointer to 8-byte character bitmap.
 */
void LCDText_CreateChar(uint8_t location, const uint8_t charmap[8]);

/** @brief Turns the display on. */
void LCDText_On(void);

/** @brief Turns the display off. */
void LCDText_Off(void);

/**
 * @brief Prints formatted text (printf-style).
 *
 * @param fmt Format string
 * @param ... Arguments
 */
void LCDText_Printf(const char *fmt, ...);

/** @} */  // LCDTEXT
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* LCDTEXT_H_ */
