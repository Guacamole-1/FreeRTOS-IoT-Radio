
#include "menu_rtos.h"
#include "Menu.h"
#include "base.h"
#include "clock.h"
#include "time_sync.h"
#include "wifi_rtos.h"
#include "display.h"
#include "FreeRTOS.h"
#include "projdefs.h"
#include "task.h"
#ifndef SSID
#define SSID "HOH"
#define PASS "spaghetto"
#endif

static void Menu_SyncClock(){
	time_t __time;
	DISPLAY_Clear();
	DISPLAY_Write("Syncronizing\ntime..");
	if(!TIME_SYNC_RequestUnixTime(&__time)){
		DISPLAY_Clear();
		DISPLAY_Write("Could not connect to NTP server");
		vTaskDelay(pdMS_TO_TICKS(2000));
		return;
	}else{
		DISPLAY_Clear();
		DISPLAY_Write("Time synchronized");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
	CLOCK_SetSeconds(__time);
}

static void Menu_ConnectAP(){
	DISPLAY_Clear();
	DISPLAY_Write("Connecting to AP: \"" SSID "\"..");
	if(WIFI_RTOS_ConnectAp(SSID, PASS) != SUCCESS){
		DISPLAY_Clear();
		DISPLAY_Write("Cannot connect to AP: " SSID);
		vTaskDelay(pdMS_TO_TICKS(2000));
	}else{
		DISPLAY_Clear();
		DISPLAY_Write("Connected to AP: " SSID);
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
}

void Menu_Maintenance(){
    Cursor c = {1,0};
    const char* const list[] = {"Edit Calendar","Edit Clock","Del. Freq.","Del. Volume","Sync Clock","Connect Wifi"};
    action funcs[] =  {Calendar_Menu,Clock_Menu,Menu_FreqDel,Menu_VolDel,Menu_SyncClock,Menu_ConnectAP};
    Spinner sp    =   {list,sizeof(funcs)/sizeof(funcs[0]),funcs};
    Field field   =   INIT_FIELDSPINNER(c,&sp);
    Menu_LcdSetup(BLINK_OFF|CURSOR_OFF);
    Menu_SM(&field,false,false);
}
