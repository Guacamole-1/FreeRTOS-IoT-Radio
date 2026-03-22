/*
 * menu.c
 *
 *  Created on: 6 Nov 2025
 *      Author: mcdnv
 */
#include "Menu.h"
#include "Delay.h"
#include "LCD.h"
#include "RTC.h"
#include "Nav7Btn.h"
#include "Fields/Field.h"
#include "Fields/Spinner.h"
#include "Fields/DateField.h"
#include "Fields/StrField.h"
#include "Fields/Menu_Field.h"
#include "Radio.h"

static Radio_Data radio_data;
// inicializa o menu
void Menu_Init(Radio_Data Rdata){
    DELAY_Init();
    NAVBTN_Init();
    LCDText_Init();
    RTC_Init(0);
    //tm temp = {.tm_year=2025-1900, .tm_mon=12-1, .tm_mday=19, .tm_yday=353, .tm_wday=4, .tm_hour=15, .tm_min=30, .tm_sec=0};
    //19 dezembro 2025 15:30, quando começam as férias de natal!! :P
    RTC_SetTimeDate(&Rdata.time);
    radio_data = Rdata;
    Init_Radio(&radio_data);
    //lcd_write8(&CURSOR_BLINK);
}

// menu state machine
// returns false on cancel and true on save
bool Menu_SM(Field* field,bool always_render,bool main_state){
    NAVBTN_TypeDef btn;
    bool render = true;
    field->init(field);
    while(1){
        btn = NAVBTN_Pressed();
        switch (btn)
        {
            case (NAVBTN_UP):
            case (NAVBTN_DOWN):
                field->step(field,btn == NAVBTN_UP);
                render = true;
                break;
            case (NAVBTN_LEFT):
            case (NAVBTN_RIGHT):
                field->shift(field,btn == NAVBTN_RIGHT,0);
                render = true;
                break;
            case(NAVBTN_ENTER):
                render = true;
                if (main_state)
                {
                    Menu_Maintenance();
                    break;
                }
            case(NAVBTN_CENTER):
                render = true;
                field->save(field);
                if (!main_state)
                {
                    return true;
                }
                break;
            case(NAVBTN_BACK):
                render = true;
                if (!main_state)
                {
                    field->cancel(field);
                    return false;
                }
                break;
            default:
                break;
        }
        if (render || always_render)
        {
            field->render(field,true);
            render = false;
        }
        DELAY_Milliseconds(50);
    }
    return false;
}



void Calendar_Menu(){
    DateField d = {"%a %d %b %Y"};
    Cursor c = (Cursor){0,0};
    Field date = INIT_DATEFIELD(c,&d);
    date.init(&date);
    lcd_write8(&CURSOR_SETUP(BLINK_ON|CURSOR_ON));
	LCDText_Clear();
    Menu_SM(&date,false,false);
}

void Clock_Menu(){ 
    DateField d = {"%H:%M:%S"};
    Cursor c = (Cursor){4,0};
    Field date = INIT_DATEFIELD(c,&d);
    date.init(&date);
    lcd_write8(&CURSOR_SETUP(BLINK_ON|CURSOR_ON));
	LCDText_Clear();
    Menu_SM(&date,false,false);
}
void empty_func(){
    return; 
}
static void _Freq_Del(){
    Radio_Data data;
    if(Get_RData(&data) == CMD_SUCCESS){
        data.channel = 0;
        Save_RData(&data);
    }
}

static void _Vol_Del(){
    Radio_Data data;
    if(Get_RData(&data) == CMD_SUCCESS){
        data.volume = 0;
        Save_RData(&data);
    }
}


static void Freq_Del(){
    Confirmation_Menu("Del. freq. data?",_Freq_Del,empty_func);
}

static void Vol_Del(){
    Confirmation_Menu("Del. vol. data?",_Vol_Del,empty_func);
}

void Confirmation_Menu(char* Conf_text, action yes, action no){
    LCDText_Clear();
    lcd_write8(&CURSOR_SETUP(BLINK_ON|CURSOR_ON));
    
    Cursor c = {0,0};
    const char* const strs[] = {Conf_text,"Yes","No"};
    Cursor pos[] = {(Cursor){LCDText_Center(Conf_text),0}, (Cursor){3,1}, (Cursor){12,1}};
    action funcs[] = {yes,no};

    StrField sf = (StrField){strs,pos,funcs,pos+1,3,2};
    Field field = INIT_STRFIELD(c,&sf);

    Menu_SM(&field,false,false);
}

void Menu_Maintenance(){
    Cursor c = {3,0};
    const char* const list[] = {"Calendar","Clock","Del. Freq.","Del. Volume"};
    action funcs[] =  {Calendar_Menu,Clock_Menu,Freq_Del,Vol_Del}; 
    Spinner sp    =   {list,4,funcs};
    Field field   =   INIT_FIELDSPINNER(c,&sp);
    LCDText_Clear();
    lcd_write8(&CURSOR_SETUP(BLINK_OFF|CURSOR_OFF));

    Menu_SM(&field,false,false);
    
}

void Main_Menu(){
    Cursor c = {0};
    DateField d = {0};
    Field datefield = INIT_DATEFIELD(c,&d);

    Menu_args args = {"%d/%m/%Y v:%v %H:%M:%S %fMHz",datefield,radio_data};
    Field f = INIT_MENUFIELD(c,&args);
    lcd_write8(&CURSOR_SETUP(BLINK_OFF|CURSOR_OFF));
	LCDText_Clear();
    Menu_SM(&f,true,true);
    datefield.cancel(&datefield); // to avoid future memory leaks
}
