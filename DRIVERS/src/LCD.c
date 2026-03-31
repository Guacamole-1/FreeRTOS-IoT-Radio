/**
* @file LCD.c
* @brief This source file implements the API for controlling the LCD
* @version 2
* @date 26 Out 2025
* @author Grupo06
*
*/
#include "Delay.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "LPC17xx.h"
#include "LCD.h"
#include <stdarg.h>


static Cursor CURSOR_POS; // cursor position x,y (columns, rows)

int LCDText_Center(const char *text)
{
	int len = strlen(text);
    return (MAX_COLUMNS+1 - len) / 2;
}

void lcd_write(lcd_t* LCD){ // writes high, then low
   LCD->E = 0;
   GPIO2->FIOPIN0 = LCD->data;
   DELAY_Microseconds(1);
   LCD->E = 1;
   GPIO2->FIOPIN0 = LCD->data;
   DELAY_Microseconds(1);
   GPIO2->FIOPIN0 = 0;
   DELAY_Microseconds(40);
   LCD->E = 0;
}


void lcd_write8(lcd16_t* LCD16){
   lcd_t LCD = (lcd_t){.DB = TOP_4(LCD16->DB), .RS = LCD16->RS};
   lcd_write(&LCD);
   LCD.DB = BOTTOM_4(LCD16->DB);
   lcd_write(&LCD);
   DELAY_Microseconds(50);
}


// must first have DELAY_init!!
void LCDText_Init(void){
   lcd_t LCD = {0};
   lcd16_t LCD16 = {0};
   CURSOR_POS = (Cursor){0,0};
   // GPIO init
   SC->PCONP |= 1 << PC_GPIO;
   GPIO2->FIODIR = LCD_BITS;
   GPIO2->FIOMASK = ~LCD_BITS;
   // timer init
   //DELAY_Init();
   //lcd init
   lcd16_t instr[] = {FUNCTION_SET3,DISPLAY_OFF,DISPLAY_CLEAR,ENTRY_MODE,DISPLAY_ON};
   int waitms[] = {50,5,1};

   LCD.DB = FUNCTION_SET;
   // repeat 3 Function sets for LCD initialization 
   for (int i = 0; i < ARRAY_SIZE(waitms); i++)
   {
      DELAY_Milliseconds(waitms[i]);
      lcd_write(&LCD);
   }
   // function set 2
   LCD.DB = FUNCTION_SET2;
   lcd_write(&LCD);

   // set up LCD Functions
   for (int i = 0; i < ARRAY_SIZE(instr); i++)
   {
      LCD16 = instr[i];
      lcd_write8(&LCD16);
      if (instr[i].DB == DISPLAY_CLEAR.DB)
      {
         DELAY_Milliseconds(6);
      }
   }
   
}

/* Escreve um caracter na posição corrente do cursor. */
void LCDText_WriteChar(char ch){
   lcd16_t LCD = {.DB = ch, .RS=1};
   lcd_write8(&LCD);
   CURSOR_POS.x++;
}


/* Escreve uma string na posição corrente do cursor. */
void LCDText_WriteString(char *str){
   for (int i = 0; i < strlen(str); i++)
   {
      if (CURSOR_POS.x > MAX_COLUMNS && CURSOR_POS.y == 0 )
      {
         LCDText_SetCursor(1,0); // put cursor in second line
      }
      if (str[i] == '\n' && CURSOR_POS.y == 0)
      {   
         LCDText_SetCursor(1,0);
      }
      else if (str[i] == '\r')
      {
         LCDText_SetCursor(CURSOR_POS.y,0); // return cursor to beginning of line
      }
      else
      {
         LCDText_WriteChar(str[i]);
      }
      
   }
   
}
// return false if position not in screen
// rows    = 0-1
// columns = 0-15
void LCDText_SetCursor(int row, int column){
   if(row > MAX_ROWS || column > MAX_COLUMNS){
      return ;
   }
   lcd16_t LCD = {.DB=SET_DDRAM_ADDR((row*ROW_OFFSET + column))};
   lcd_write8(&LCD);
   CURSOR_POS = (Cursor){column,row};
}

void LCDText_CursorSet(Cursor c){
   if(c.y > MAX_ROWS || c.x > MAX_COLUMNS){
      return ;
   }
   lcd16_t LCD = {.DB=SET_DDRAM_ADDR((c.y*ROW_OFFSET + c.x))};
   lcd_write8(&LCD);
   CURSOR_POS = c;
}

void LCDText_Clear(void){
   CURSOR_POS = (Cursor){0,0};
   lcd_write8(&DISPLAY_CLEAR);
   DELAY_Milliseconds(6);
}


void __LCDText_Printf(char *fmt, int count, ...){
   char str[MAX_CHARS+1];
   va_list args;
   va_start(args, count); 
   vsnprintf(str,MAX_CHARS,fmt,args);
   LCDText_WriteString(str);
   va_end(args);
   
}
