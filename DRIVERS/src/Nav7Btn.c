/**
* @file Nav7Btn.c
* @brief This source file implements the API for controlling the 7 buttons
* @version 1.1
* @date 27 Out 2025
* @author Grupo06
*/
#include "Nav7Btn.h"
#include "LPC17xx.h"
#include <stdbool.h>
#define PINCON LPC_PINCON
#define PIN_MODE 0xFF << 14

#define GET_NAV_NAME(x) case NAVBTN_ ## x: \
                        return  #x;


static const NAVBTN_TypeDef nav_map[][4] = {
    {NAVBTN_BACK,NAVBTN_CENTER,NAVBTN_ENTER},
    {NAVBTN_UP,NAVBTN_DOWN,NAVBTN_LEFT,NAVBTN_RIGHT}
};

static const uint8_t input_map[][4] = {
    {BIT_POS(1),BIT_POS(2),BIT_POS(3)},
    {BIT_POS(0),BIT_POS(1),BIT_POS(2),BIT_POS(3)}
};

// returns a string that represents the NAVBTN_TypeDef
char* NAVBTN_GetName(NAVBTN_TypeDef nav){
    switch (nav)
    {
    GET_NAV_NAME(UP)
    GET_NAV_NAME(DOWN)
    GET_NAV_NAME(LEFT)
    GET_NAV_NAME(RIGHT)
    GET_NAV_NAME(CENTER)
    GET_NAV_NAME(BACK)
    GET_NAV_NAME(ENTER)
    default:
        return "NONE";
    }
}

/* Faz a iniciação do sistema para permitir o acesso ao teclado. */
void NAVBTN_Init(void){
    SC->PCONP |= 1 << PC_GPIO;
    GPIO0->FIODIR |= COL_BITS;
    //GPIO0->FIOMASK = ~NAV_BITS; // change this
    PINCON->PINMODE1 |= PIN_MODE;
}

// sets the columns output and reads the rows
static void read_data(Nav7Btn* btn){

	uint32_t col_mask = btn->columns << COL0_PIN;

    GPIO0->FIOCLR = COL_BITS;
    GPIO0->FIOSET = col_mask;

    btn->rows = (GPIO0->FIOPIN & ROW_BITS) >> ROW0_PIN;
	GPIO0->FIOCLR = COL_BITS;

}

/* Lê o teclado e devolve a primeira tecla detetada.
Se nenhuma tecla estiver pressionada, devolve NAVBTN_NONE.
Não é bloqueante. */
NAVBTN_TypeDef NAVBTN_Read(void){

    uint8_t row_size;
    Nav7Btn btn = {0};

    for (uint8_t col_num = 0; col_num < 2; col_num++) { // read the two columns

        btn.columns = 1 << col_num;
        read_data(&btn);
        row_size = col_num == 0 ? 3 : 4;

        for (uint8_t row_num = 0; row_num < row_size; row_num++){
            if (btn.rows & input_map[col_num][row_num])// >> row_num) no need to shift because if evaluates every non zero number to true
            {
                return nav_map[col_num][row_num];
            }
        }
    }
    return NAVBTN_NONE;
}
/* Igual a NAVBTN_Read(), mas se a tecla se manteve pressionada entre duas
chamadas consecutivas (não permite a repetição).
Não é bloqueante. */
NAVBTN_TypeDef NAVBTN_Pressed(void){
    static NAVBTN_TypeDef last = NAVBTN_NONE;
    static bool was_pressed = false;

    NAVBTN_TypeDef current = NAVBTN_Read();
    if ( current == last && was_pressed == false)
    {
        was_pressed = true;
        return last;
    }
    else if (current != last)
    {
        was_pressed = false;
    }
    
    last = current;
    return NAVBTN_NONE;
    
}
