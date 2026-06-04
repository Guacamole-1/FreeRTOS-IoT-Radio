/**
* @file StrField.c
* @brief This source file implements the API for the strField funcs
* @version 1
* @date 14 Dec 2025
* @author Grupo06
*
*/

#include "Fields/StrField.h"
#include <string.h>

#ifdef FREE_RTOS
#include "clock.h"
#include "display.h"
#endif

FieldInitFunc(strField_init){
    StrField* sf = (StrField *)f->data;
    sf->_index = 0;
    return;
}

FieldRenderFunc(strField_render){
    StrField* sf = (StrField *)f->data;
#ifndef FREE_RTOS
    LCDText_Clear();
#else
	DISPLAY_Clear();
#endif

    for (int i = 0; i < sf->_strings_size; i++)
	{
#ifndef FREE_RTOS
		LCDText_CursorSet(sf->strings_pos[i]);
		LCDText_WriteString(sf->strings[i]);
#else
		DISPLAY_CursorSet(sf->strings_pos[i]);
		DISPLAY_Write(sf->strings[i]);
#endif
    }
    if (focused) f->cursor(f);
}

FieldCursorFunc(strField_cursor){
    StrField* sf = (StrField *)f->data;
#ifndef FREE_RTOS
    LCDText_CursorSet(sf->action_pos[sf->_index]);
#else
    DISPLAY_CursorSet(sf->action_pos[sf->_index]);
#endif
}

FieldStepFunc(strField_step){
    return false;
}

FieldShiftFunc(strField_shift){
    StrField* sf = (StrField *)f->data;
    bool bey = false;
    sf->_index += right ? 1 : -1 ;
    if (sf->_index >= sf->_actions_size )
    {
        sf->_index = 0;
        bey = true;

    } else if (sf->_index < 0)
    {
        sf->_index = sf->_actions_size-1;
        bey = true;
    }
    return bey;
}

FieldSaveFunc(strField_save){
    StrField* sf = (StrField *)f->data;
    sf->action_list[sf->_index]();
}

FieldCancelFunc(strField_cancel){
    return;
}



