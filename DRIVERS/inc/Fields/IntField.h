/**
* @file IntField.h
* @brief This header file provides the API for the IntField Functions
* @version 1
* @date 11 Nov 2025
* @author Grupo06
*
*/

/**
* @ingroup Fields
* @defgroup IntField 
* @brief This module provides helper functions for the IntField struct
* @{
*/

#ifndef FIELDS_INTFIELD_H_
#define FIELDS_INTFIELD_H_
#include <stdint.h>
#include "Fields/Field.h"

typedef struct
{
    int        *value;         
    int        _display_val;     //saved value to be written to value on FieldSave
    int        min, max;
    uint8_t    digits;          // e.g., 3 for 000..999
    uint8_t    index;           // active digit 0..digits-1 , 0 = most-significant
    char       fmt[10];          // format, ex. "%03d" -> ( 1 = "001")
} IntField;       


FieldRenderFunc(int_render);

FieldCursorFunc(int_cursor);

FieldStepFunc(int_step);

FieldShiftFunc(int_shift);

FieldSaveFunc(int_save);

FieldCancelFunc(int_cancel);

FieldInitFunc(int_init);


#endif /* FIELDS_INTFIELD_H_ */

/**
* @}
*/
