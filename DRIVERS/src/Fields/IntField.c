#include "Fields/Field.h"
#include "Fields/IntField.h"
#include <stdio.h>
#include <math.h>

#ifdef FREE_RTOS
	#include "display.h"
#endif

FieldInitFunc(int_init){
    IntField *field = (IntField*)f->data;
    field->_display_val = clamp(*field->value,field->min,field->max);
    field->index = 0;
}


FieldRenderFunc(int_render){

    IntField *field = (IntField*)f->data;
#ifndef FREE_RTOS
    char buf[MAX_CHARS+1] = {0};  // +1 for null terminator
    snprintf(buf, sizeof(buf), field->fmt, field->_display_val);
    LCDText_CursorSet(f->pos);
    LCDText_WriteString(buf);
#else
	DISPLAY_CursorSet(f->pos);
	DISPLAY_Printf(field->fmt,field->_display_val);
#endif
    if (focused) f->cursor(f);
}

FieldCursorFunc(int_cursor){

    IntField *field = (IntField*)f->data;

#ifndef FREE_RTOS
    LCDText_SetCursor(f->pos.y, f->pos.x + field->index);
#else
	DISPLAY_SetCursor(f->pos.y, f->pos.x + field->index);
#endif
}

FieldStepFunc(int_step){

    IntField *field = (IntField*)f->data;

    int step = pow(10,(field->digits-field->index-1));
    int new_value = (up ? step : -step) + field->_display_val;
    int clamped = clamp(new_value,field->min,field->max);

    if (new_value != clamped)
    {
        return false;
    }

    field->_display_val = clamped;
    return true;

}

FieldShiftFunc(int_shift){
    IntField *field = (IntField*)f->data;
    if (max)
    {
        field->index = right ? field->digits-1 : 0;
        return false;
    }

    int next = field->index + (right ? 1 : -1);

    if (next < 0)
    {
        next = field->digits-1;
    }
    else if (next >= field->digits)
    {
        next = 0;
    }

    field->index = next;
    return (next == 0 && right) || ((next == field->digits - 1) && !right);
}

FieldSaveFunc(int_save){
    IntField *field = (IntField*)f->data;
    *field->value = field->_display_val;
    return;
}

FieldCancelFunc(int_cancel){
    IntField *field = (IntField*)f->data;
    field->_display_val = *field->value;
    return;
}

