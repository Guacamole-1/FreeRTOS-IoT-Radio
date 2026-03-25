/**
* @file Spinner.c
* @brief This source file implements the API for the Spinner struct
* @version 1
* @date 22 Nov 2025
* @author Grupo06
*
*/

#include "Fields/Spinner.h"
#include "LCD.h"
#include <string.h>
#include <stdio.h>

FieldInitFunc(spinner_init){
    Spinner *s = (Spinner *)f->data;
    s->_index = 0;
    return;
}

FieldRenderFunc(spinner_render){
    Spinner *s = (Spinner *)f->data;
    char buf[MAX_COLUMNS+2];
    LCDText_Clear();
    snprintf(buf,sizeof(buf),"%c %s",ARROW_CHAR,s->list[s->_index]);
    //center_text(buf,0);
    LCDText_CursorSet(f->pos);
    LCDText_WriteString(buf);

    snprintf(buf,sizeof(buf),"  %s",s->list[s->_index >= s->list_size - 1 ? 0 : s->_index+1]);
    //center_text(buf,1);
    LCDText_SetCursor(f->pos.y+1,f->pos.x);
    LCDText_WriteString(buf);
    
}

FieldCursorFunc(spinner_cursor){
    return;
}

FieldStepFunc(spinner_step){
    Spinner *s = (Spinner *)f->data;
    s->_index += up ? -1 : 1 ;
    if (s->_index >= s->list_size )
    {
        s->_index = 0;
    } else if (s->_index < 0)
    {
        s->_index = s->list_size-1;
    }
    
    return true;
}

FieldShiftFunc(spinner_shift){
    return spinner_step(f,!right);
}


FieldSaveFunc(spinner_save){
    Spinner *s = (Spinner *)f->data;
    s->action_list[s->_index]();
}

FieldCancelFunc(spinner_cancel){
    return;
}
