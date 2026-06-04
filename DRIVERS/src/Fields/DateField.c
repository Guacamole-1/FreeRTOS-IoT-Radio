#include "Fields/DateField.h"

#include <string.h>
#include <stdlib.h>
#include "RTC.h"
#include "LCD.h"
#include <stdio.h>
#include <ctype.h>

#ifdef FREE_RTOS
#include "clock.h"
#include "display.h"
#endif


void free_DateField(DateField* d){
    if (!d || !d->_fields) return;

    for (int i = 0; i < d->_field_count; ++i) {
        if (!d->_fields[i]) continue;
        FREE(d->_fields[i]->data);
        FREE(d->_fields[i]);
    }
    FREE(d->_fields);
    d->_fields = NULL;
    d->_field_count = 0;
}

int find_spec_length(char ch)
{
    static const int spec_lengths[] = {
        3, // a (max size)
        9, // A (max size)
        3, // b (max size)
        9, // B (max size)
        2, // d
        2, // H
        2, // I
        3, // j
        2, // m
        2, // M
        2, // S
        2, // U
        1, // w
        2, // W
        2, // y
        4  // Y
    };
static const char* specifiers = "aAbBdHIjmMSUwWyY";

    for (int f = 0; specifiers[f] != '\0'; f++)
    {
        if (specifiers[f] == ch)
        {
            return spec_lengths[f];
        }
    }
    return -1;

}

bool is_leap(int year){
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Max_dom(int year,int month){
    bool leap = is_leap(year);

    if (month == 2) // february
    {
        return leap ? 29 : 28;

    }else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return 30;
    }
    else
    {
        return 31;
    }

}


// supports SMHdbBmYyjwaA
FieldInitFunc(date_init)
{
    DateField *d = (DateField *)f->data;
	#ifndef FREE_RTOS
		RTC_GetTimeDate(&d->_date);
	#else
		CLOCK_GetTimeDate(&d->_date);
	#endif
    d->_field_index = 0;

    int year  = d->_date.tm_year + 1900;
    int month = d->_date.tm_mon + 1;
    int max_dom = Max_dom(year, month);

    // -------- 1) Scan format string and collect specifiers --------
    Cursor pos[MAX_CHARS];
    int    lengths[MAX_CHARS];
    char   specs[MAX_CHARS];     // specifier char per field

    int specifier_count = 0;
    int pos_index = 0;

    int col = 0;
    int row = 0;

    for (int i = 0; d->fmt[i] != '\0'; i++)
    {
        char ch = d->fmt[i];

        if (ch == '\n') {
            // explicit newline in format string
            col = 0;
            row++;
            continue;
        }

        if (ch == '%' && d->fmt[i+1] != '\0') {

            char c = d->fmt[i+1];
            int len;

            if (c == '%') {
                col++;
                if (col >= MAX_COLUMNS) {
                    col = 0;
                    row++;
                }
                i++;
                continue;
            }

            if (specifier_count >= MAX_CHARS)
                break; 

            pos[pos_index] = (Cursor){ col, row };
            if (tolower(c) == 'a' || tolower(c) == 'b')
            {
                col += find_spec_length(c);
                len = 1;
            }
            else{
                len = find_spec_length(c);
                col += len;
            }

            lengths[pos_index] = len;
            specs[pos_index]   = c;

            pos_index++;
            specifier_count++;

            if (col >= MAX_COLUMNS) {
                col = 0;
                row++;
            }

            i++; 
        } else {
            col++;
            if (col >= MAX_COLUMNS) {
                col = 0;
                row++;
            }
        }
    }
    // allocate Fields
    d->_field_count = specifier_count;
	d->_fields = CALLOC(specifier_count, sizeof(Field *));

    if (!d->_fields) {
        d->_field_count = 0;
        return;
    }

    for (int i = 0; i < specifier_count; ++i)
    {
        d->_fields[i] = MALLOC(sizeof(Field));
        if (!d->_fields[i]) {
            continue;
        }

        *(d->_fields[i]) = (Field){
            pos[i],
            int_init,
            int_render,
            int_cursor,
            int_step,
            int_shift,
            int_save,
            int_cancel,
            NULL
        };
    }


    // create intfields
    //aAbBdHIjmMSUwWyY
    for (int index = 0; index < specifier_count; ++index)
    {
        char spec = specs[index];
        int *value_ptr = NULL;
        int min = 0, max = 0;

        switch (spec)
        {
        case 'S': // seconds (00-59)
            value_ptr = &d->_date.tm_sec;
            min = 0;  max = 59;
            break;

        case 'M': // minutes (00-59)
            value_ptr = &d->_date.tm_min;
            min = 0;  max = 59;
            break;

        case 'H': // hours (00-23)
            value_ptr = &d->_date.tm_hour;
            min = 0;  max = 23;
            break;

        case 'd': // day of month (01-31)
            value_ptr = &d->_date.tm_mday;
            min = 1;  max = max_dom;
            break;

        case 'b':
        case 'B':
        case 'm': // month number; tm_mon is 0-11
            value_ptr = &d->_date.tm_mon;
            min = 0;  max = 11;
            break;

        case 'Y': // full year, tm_year = year - 1900
            value_ptr = &d->_date.tm_year;
            min = 0;  max = 4095 - 1900;
            break;

        case 'y': // 2-digit year (00-99)
            value_ptr = &d->_date.tm_year;
            min = 0;  max = 99;
            break;

        case 'j': // day of year (0-365)
            value_ptr = &d->_date.tm_yday;
            min = 0;  max = is_leap(d->_date.tm_year) ? 365 : 364;
            break;

        case 'w': // day of week (0-6, Sunday = 0)
        case 'a':
        case 'A':
            value_ptr = &d->_date.tm_wday;
            min = 0;  max = 6;
            break;

        default:
            continue;
        }

        if (!value_ptr)
            continue;

        IntField *Intf = MALLOC(sizeof(IntField));
        if (!Intf)
            continue;

        d->_fields[index]->data = Intf;

        *Intf = (IntField){value_ptr,*value_ptr,min,max,lengths[index],0}; 

        snprintf(Intf->fmt, sizeof(Intf->fmt), "%%0%dd", lengths[index]);
    }
}

FieldRenderFunc(date_render){
    DateField* d = (DateField*)f->data;
    IntField* field = (IntField*)d->_fields[d->_field_index]->data;
    char buf[MAX_CHARS+1];

    //RTC_GetTimeDate(&d->_date);

    int temp = *field->value;

    *field->value = field->_display_val;
    strftime(buf,sizeof(buf),d->fmt,&d->_date);
    *field->value = temp;
#ifndef FREE_RTOS
    LCDText_CursorSet(f->pos);
    LCDText_WriteString(buf);
#else
	DISPLAY_CursorSet(f->pos);
	DISPLAY_Write(buf);
#endif
    if (focused) f->cursor(f);
}

FieldCursorFunc(date_cursor){
    DateField* d = (DateField*)f->data;
    Field* field = d->_fields[d->_field_index];
    IntField* intf = (IntField*)field->data;
    Cursor c = field->pos;
 
    int x = f->pos.x + c.x + intf->index; //position of Field + position of current Intfield + index
    int y = f->pos.y + c.y;
    bool nl = x > MAX_COLUMNS;
#ifndef FREE_RTOS
    LCDText_SetCursor(nl ? 1 : y, nl ? x - 16 : x); // if it goes beyond max, wrap
#else
	DISPLAY_SetCursor(nl ? 1 : y, nl ? x - 16 : x);
#endif
}

FieldStepFunc(date_step)
{
    DateField *d = (DateField *)f->data;
    Field     *field = d->_fields[d->_field_index];
    IntField  *cur   = (IntField *)field->data;

    bool stepped = field->step(field, up);
    if (!stepped)
        return false;

    *cur->value = cur->_display_val;

    int year  = d->_date.tm_year + 1900;
    int month = d->_date.tm_mon + 1;
    int day = d->_date.tm_mday;

    int max_dom = Max_dom(year, month);

    if (day > max_dom) {
        d->_date.tm_mday = max_dom;


        for (int i = 0; i < d->_field_count; ++i) {
            Field    *fi_field = d->_fields[i];
            if (!fi_field || !fi_field->data) continue;

            IntField *fi = (IntField *)fi_field->data;
            if (fi->value == &d->_date.tm_mday) {
                fi->_display_val = d->_date.tm_mday;
                break;
            }
        }
    }

    // return true if something actually changed
    return true;
}

FieldShiftFunc(date_shift){
    DateField* d = (DateField*)f->data;
    Field* field = d->_fields[d->_field_index];

    if (max)
    {
        d->_field_index = right ? d->_field_count-1 : 0;
        return true;
    }

    bool mv_next = field->shift(field,right,max);
    if (!mv_next) return false;
    field->save(field);
    int next = d->_field_index + (right ? 1 : -1);

    if (next < 0)
    {
        next = d->_field_count-1;
    }
    else if (next >= d->_field_count)
    {
        next = 0;
    }

    d->_field_index = next;

    Field* nxt = d->_fields[next];
    nxt->shift(nxt,!right,true);

    return (next == 0 && right) || ((next == d->_field_count - 1) && !right);
}
//TODO change init display value in date_render func
FieldSaveFunc(date_save){
    DateField* d = (DateField*)f->data;
    //RTC_GetTimeDate(&d->_date);

    for (int i = 0; i < d->_field_count; i++) // save changes (write into tm struct)
    {
        d->_fields[i]->save(d->_fields[i]);
    }
	#ifndef FREE_RTOS
		RTC_SetTimeDate(&d->_date);
	#else
		CLOCK_SetTimeDate(&d->_date);
	#endif
    free_DateField(d);
    d->_date = (tm){0};
    d->_field_count = 0;
    d->_field_index = 0;
}

FieldCancelFunc(date_cancel){
    DateField* d = (DateField*)f->data;
    free_DateField(d);
    d->_date = (tm){0};
    d->_field_count = 0;
    d->_field_index = 0;
}
