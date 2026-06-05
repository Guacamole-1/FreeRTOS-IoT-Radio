/*
 * RTC.c
 *
 *  Created on: 4 Nov 2025
 *      Author: mcdnv
 */
#include "RTC.h"

const char* week_days[] = {"Mon", "Tue", "Wed", "Thu","Fri","Sat","Sun"}; 

void RTC_Init(time_t seconds){
    SC->PCONP |= 1 << PC_RTC;
    RTC->CCR = 0b10; // clock reset
    RTC->CCR = 0; // clock disabled
    tm* time = localtime(&seconds);
    RTC_SetTimeDate(time);
    RTC->CCR = 1; //clock enabled
}

//limits values to a range between min and max
int clamp(int value,int min,int max){
    if (value > max){
        return max;
    } else if (value < min)
    {
        return min;
    }
    return value;
    
}

void RTC_SetTimeDate(tm *dateTime){
    RTC->CCR = 0b10; // clock reset
    RTC->CCR = 0b0; // disable reset
    int max_dom;
    int year  = dateTime->tm_year + 1900;
    int month = dateTime->tm_mon + 1;
    bool is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    
    if (month == 2) // february
    {
        max_dom = is_leap ? 29 : 28;

    }else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        max_dom = 30;
    }
    else
    {
        max_dom = 31;
    }
    
    LPC_RTC->YEAR  = clamp(year, 0, 4095);
    LPC_RTC->MONTH = clamp(month, 1, 12);
    LPC_RTC->DOM   = clamp(dateTime->tm_mday, 1, max_dom);                 
    LPC_RTC->DOW   = clamp(dateTime->tm_wday, 0, 6);
    LPC_RTC->DOY   = clamp(dateTime->tm_yday + 1, 1, 365 + is_leap);            
    LPC_RTC->HOUR  = clamp(dateTime->tm_hour, 0, 23);
    LPC_RTC->MIN   = clamp(dateTime->tm_min, 0, 59);
    LPC_RTC->SEC   = clamp(dateTime->tm_sec, 0, 59);

	RTC->CCR = 1; //clock enabled
}



static tm get_time(){
    uint32_t time;
    tm dateTime;
    time = RTC->CTIME2;
	dateTime.tm_yday = GET_BITS(time,0,11);

	time = RTC->CTIME1;
	dateTime.tm_mday = GET_BITS(time,0,4);
	dateTime.tm_mon = GET_BITS(time,8,11);
	dateTime.tm_year = GET_BITS(time,16,27);

	time = RTC->CTIME0;
	dateTime.tm_sec = GET_BITS(time,0,5);
	dateTime.tm_min = GET_BITS(time,8,13);
	dateTime.tm_hour = GET_BITS(time,16,20);
	dateTime.tm_wday = GET_BITS(time,24,26);
    return dateTime;
}

void RTC_GetTimeDate(tm *dateTime){
    tm first = get_time();
	tm sec = get_time();
    if ((sec.tm_mday != first.tm_mday) ||
		(sec.tm_mon  != first.tm_mon)  ||
		(sec.tm_year != first.tm_year) ||
		(sec.tm_yday != first.tm_yday)) //23:59:59 31 dez 2025 
    { //00:00:00 1 jan 2026
        *dateTime = get_time();
    }else{
        *dateTime = sec;
    }
    dateTime->tm_year -= 1900;
    dateTime->tm_mon  -= 1;
    dateTime->tm_yday -= 1;
}

void RTC_SetSeconds(time_t seconds){
	tm* time = localtime(&seconds);
	RTC_SetTimeDate(time);
}

time_t RTC_GetSeconds(void){
    tm date;
    RTC_GetTimeDate(&date);
	return mktime(&date);
}

