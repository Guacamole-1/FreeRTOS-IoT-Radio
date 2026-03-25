/**
* @file Spinner.h
* @brief This header file provides the API for the StrField Functions
* @version 1
* @date 15 Dec 2025
* @author Grupo06
*
*/

/**
* @ingroup Fields
* @defgroup StrField
* @brief This module provides helper functions for the StrField struct
* @{
*/


#ifndef FIELDS_STRFIELD_H_
#define FIELDS_STRFIELD_H_

#include "Fields/Field.h"
#include "Fields/Spinner.h"

#define INIT_STRFIELD(pos,data) INIT_FIELD((pos),(data),strField)

typedef struct
{
    const char* const*  strings;
    Cursor*             strings_pos;
    action*             action_list;       
    Cursor*             action_pos;
    uint8_t             _strings_size;
    uint8_t             _actions_size;
    uint8_t             _index;

}StrField;

FieldInitFunc(strField_init);
FieldRenderFunc(strField_render);
FieldCursorFunc(strField_cursor);
FieldStepFunc(strField_step);
FieldShiftFunc(strField_shift);
FieldSaveFunc(strField_save);
FieldCancelFunc(strField_cancel);


#endif /* FIELDS_STRFIELD_H_ */

/**
* @}
*/