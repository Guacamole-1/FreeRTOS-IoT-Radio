/**
* @file DateField.h
* @brief This header file provides the API for the DateField Functions
* @version 1
* @date 13 Nov 2025
* @author Grupo06
*
*/

/**
* @ingroup Fields
* @defgroup DateField 
* @brief This module provides helper functions for the DateField struct
* @{
*/
#ifndef FIELDS_DATEFIELD_H_
#define FIELDS_DATEFIELD_H_

#include "Fields/Field.h"
#include "Fields/IntField.h"

#include <time.h>

/**
 * @brief creates a Field filled with DateField functions
 * 
 */
#define INIT_DATEFIELD(pos,data) INIT_FIELD((pos),(data),date)

typedef struct 
{
    char      fmt[MAX_CHARS+1];
    tm        _date;
    int       _field_index;
    int       _field_count;
    Field**   _fields;

}DateField;


FieldRenderFunc(date_render);

FieldCursorFunc(date_cursor);

FieldStepFunc(date_step);

FieldShiftFunc(date_shift);

FieldSaveFunc(date_save);

FieldCancelFunc(date_cancel);

FieldInitFunc(date_init);

#endif /* FIELDS_DATEFIELD_H_ */

/**
* @}
*/
