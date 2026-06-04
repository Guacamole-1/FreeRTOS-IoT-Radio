/**
* @file Field.h
* @brief This header file provides the API for the Field Functions
* @version 1
* @date 11 Nov 2025
* @author Grupo06
*
*/

/**
* @addtogroup DRIVERS
* @{
*/

/**
* @defgroup Fields Fields
* @warning Because of the macros doxygen cant find the functions on headers except Field.h
* @{
*/

/**
* @defgroup Field
* @ingroup DRIVERS
* @brief This module provides helper functions for the Field struct
* @{
*/


#ifndef FIELDS_FIELD_H_
#define FIELDS_FIELD_H_

#include "RTC.h"
#include "LCD.h"
#include <stdbool.h>

typedef struct Field Field;


//For FreeRTOS functions for the linker and preprocessor yurr
#ifdef FREE_RTOS
	void * pvPortMalloc( size_t xWantedSize ) ;
	void * pvPortCalloc( size_t xNum,
						size_t xSize ) ;
	void vPortFree( void * pv ) ;
	#define FREE vPortFree
	#define CALLOC pvPortCalloc
	#define MALLOC pvPortMalloc
	#else
	#define FREE free
	#define CALLOC calloc
	#define MALLOC malloc
#endif


/**
 * @brief function to set rendering behavior
 * @return void
 */
#define FieldRenderFunc(name)  void name(Field *f, bool focused)
/**
 * @brief function to set the user cursor position
 * @return void
 */
#define FieldCursorFunc(name) void name(Field *f)
/**
 * @brief function to step (increase/decrease)
 * @param up whether to increase or decrease
 * @return true or false depending if a step was performed
 */
#define FieldStepFunc(name)   bool  name(Field *f, bool up)
/**
 * @brief function to shift user cursor position
 * @param right specifies right or left direction
 * @param max specifies whether to go to the last field in the right direction
 * @sa right
 * @return true or false depending on whether it shifted to the beyond the bounds of the field
 */
#define FieldShiftFunc(name)  bool name(Field *f, bool right, bool max)
/**
 * @brief function to save changes
 * @return void
 */
#define FieldSaveFunc(name)   void name(Field *f)
/**
 * @brief function to exit and not save changes
 * @return void
 */
#define FieldCancelFunc(name) void name(Field *f)
/**
 *
 * @brief Init function
 * @return void
 */
#define FieldInitFunc(name) void name(Field *f)

typedef FieldInitFunc   ((  *FieldInit    ));
typedef FieldRenderFunc ((  *FieldRender  ));
typedef FieldCursorFunc ((  *FieldCursor  ));
typedef FieldStepFunc   ((  *FieldStep    ));
typedef FieldShiftFunc  ((  *FieldShift   ));
typedef FieldSaveFunc   ((  *FieldSave    ));
typedef FieldCancelFunc ((  *FieldCancel  ));

/**
 * @brief init macro for automating functions,
 *
 * type is what comes before _render, _cursor, etc
 *
 * ex. Field f = INIT_FIELD(cursor,intfield,int)
 *
 */
#define INIT_FIELD(pos,data,type) (Field){(pos),(type ## _init), (type ## _render), (type ## _cursor), \
        (type ## _step), (type ## _shift), (type ## _save),(type ## _cancel),(data)}


struct Field{
    Cursor         pos;
    FieldInit      init;
    FieldRender    render;      // function to set rendering behavior
    FieldCursor    cursor;      // function to set cursor position
    FieldStep      step;
    FieldShift     shift;       // function to set movement behavior (going left/right)
    FieldSave      save;
    FieldCancel    cancel;
    void *data;                 // field-specific data
};

#endif /* FIELDS_FIELD_H_ */

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/
