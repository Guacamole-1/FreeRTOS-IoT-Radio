/**
* @file Spinner.h
* @brief This header file provides the API for the Spinner Functions
* @version 1
* @date 22 Nov 2025
* @author Grupo06
*
*/

/**
* @ingroup Fields
* @defgroup Spinner
* @brief This module provides helper functions for the Spinner struct
* @{
*/


#ifndef FIELDS_SPINNER_H_
#define FIELDS_SPINNER_H_
#include "Fields/Field.h"

#define ARROW_CHAR '>'

/**
 * @brief creates a Field filled with Spinner functions
 * 
 */
#define INIT_FIELDSPINNER(pos,data) INIT_FIELD((pos),(data),spinner)

typedef void (* action)(void);

typedef struct
{
    const char* const* list;
    int          list_size;
    action*      action_list;       
    int          _index;

}Spinner;


FieldInitFunc(spinner_init);

FieldRenderFunc(spinner_render);

FieldCursorFunc(spinner_cursor);

FieldStepFunc(spinner_step);

FieldShiftFunc(spinner_shift);

FieldSaveFunc(spinner_save);

FieldCancelFunc(spinner_cancel);



#endif /* FIELDS_SPINNER_H_ */
/**
* @}
*/
