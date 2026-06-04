/**
* @file Menu_Field.h
* @brief This header file provides the API for the MenuField Functions
* @version 2
* @date 19 Dec 2025
* @author Grupo06
*
*/

/**
* @ingroup Fields
* @defgroup MenuField 
* @brief This module provides helper functions for the MenuField struct
* @{
*/

#ifndef MENU_FIELD_H_
#define MENU_FIELD_H_
#include "Fields/DateField.h"
#include "Radio.h"


typedef struct
{
    char        fmt[MAX_CHARS+1];
    Field       datefield;
    Radio_Data*  Rdata;
} Menu_args;    

#define INIT_MENUFIELD(pos,data) INIT_FIELD((pos),(data),Menu_Field)


FieldRenderFunc(Menu_Field_render);

FieldCursorFunc(Menu_Field_cursor);

FieldStepFunc(Menu_Field_step);

FieldShiftFunc(Menu_Field_shift);

FieldSaveFunc(Menu_Field_save);

FieldCancelFunc(Menu_Field_cancel);

FieldInitFunc(Menu_Field_init);


#endif /* MENU_FUNCS_H_ */
