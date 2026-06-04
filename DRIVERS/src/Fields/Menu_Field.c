/*
 * MenuFuncs.c
 *
 *  Created on: 17 Dec 2025
 *      Author: mcdnv
 */

//#include "Menu_Funcs.h"
#include "Fields/DateField.h"
#include "LCD.h"
#include "Radio.h"
#include "clock.h"
#include <string.h>
#include "Fields/Menu_Field.h"

#define MAX_VOL     0xF
#define MIN_VOL     0
#define MAX_CHAN    0x3FF
#define MIN_CHAN    0

#ifdef FREE_RTOS
#include "clock.h"
#include "display.h"
#include "radio_rtos.h"
#endif


// ex. "%d/%m/%Y vol:%v" = 19/4/2005 vol:5  
static void Radio_format(Field* f){
    Menu_args* args = (Menu_args*)f->data;
    Field df        = args->datefield;
    DateField* date = (DateField*)df.data;
    char * str = args->fmt;
    char * final = date->fmt;
    //int strsize = strlen(args->fmt);
    int j = 0;
    for (int i = 0; str[i] != '\0' && j < MAX_CHARS+1; i++ , j++)
    {
        if (str[i] == '%' && str[i+1] != '%' && str[i+1] != '\0')
        {
            switch (str[i+1])
            {
            case 'v':
                char vol[4] = {0}; // 1 or 2 digits + \0
                int vol_size = snprintf(vol,4,"%02d",args->Rdata->volume); // ab%dc | ab2c
                if (vol_size < 0) vol_size = 0;
                strncpy(final+j,vol,vol_size);
                i += 1;
                j += vol_size-1;
                continue;
            case 'f':
                char freq[6] = {0}; // "100.0" (5) + \0
                double frequency = CURRENT_FREQ(((double)args->Rdata->channel));
                int freq_size = snprintf(freq,6,"%.1f",frequency);
                if (freq_size < 0) freq_size = 0;

                if (j + freq_size >= MAX_CHARS ) // writing too much 12 + 4 >= 15     15-12 = 3
                {
                    int remainder = MAX_CHARS - j;
                    strncpy(final+j,freq,remainder); // write whats left of what can be written
                    j += remainder-1;
                }else{
                    strncpy(final+j,freq,freq_size);
                    j += freq_size-1;
                }
                i += 1;
                continue;
            default:
                break;
            }
        }
        final[j] = str[i];
    }
    final[j] = '\0';
}

FieldInitFunc(Menu_Field_init){
    Menu_args* args = (Menu_args*)f->data;
    Field df        = args->datefield;
    Radio_format(f);
    df.init(&df);
    return;
}
// focused could be used for a different display maybe if i got time (i dont)
FieldRenderFunc(Menu_Field_render){
    Menu_args* args = (Menu_args*)f->data;
    Field df        = args->datefield;
    DateField* d     = df.data;
    Radio_format(f);
#ifndef FREE_RTOS
    LCDText_Clear();
    RTC_GetTimeDate(&d->_date);
#else
	DISPLAY_Clear();
	CLOCK_GetTimeDate(&d->_date);
#endif
    df.render(&df,0);
}

FieldCursorFunc(Menu_Field_cursor){
    return;
}

FieldStepFunc(Menu_Field_step){
    Menu_args* args = (Menu_args*)f->data;
    int new = args->Rdata->volume + (up ? 1 : -1); //14+1 = 15

    if (new <= MAX_VOL && new >= MIN_VOL)
    {
        args->Rdata->volume = new;
#ifndef FREE_RTOS
        Set_Volume(args->Rdata,NULL);
#else
		RADIO_RTOS_SET_VOLUME(args->Rdata, NULL);
#endif
        return true;
    }
  return false;
}

FieldShiftFunc(Menu_Field_shift){
    Menu_args* args = (Menu_args*)f->data;
    int new = args->Rdata->channel + (right ? 1 : -1);

    if (new <= MAX_CHAN && new >= MIN_CHAN)
    {
        args->Rdata->channel = new;
#ifndef FREE_RTOS
        Set_Channel(args->Rdata,NULL);
#else
		RADIO_RTOS_SET_CHANNEL(args->Rdata,NULL);
#endif
        return true;
    }
    return false;
}

FieldSaveFunc(Menu_Field_save){
    Menu_args* args = (Menu_args*)f->data;
#ifndef FREE_RTOS
    Save_RData(args->Rdata);
#else
	RADIO_RTOS_SAVE_DATA(args->Rdata);
#endif
}

FieldCancelFunc(Menu_Field_cancel){
    return;
}
