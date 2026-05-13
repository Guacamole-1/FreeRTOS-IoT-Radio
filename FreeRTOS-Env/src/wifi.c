
#include "FreeRTOS.h"

/* Kernel includes. */
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* RTOS Modules includes */
#include "mutex_wrapper.h"
#include "base.h"

/* Driver includes */
#include "uart.h"

/* stdlib includes */
#include <__stdarg_va_list.h>
#include <stdarg.h>
#include <stdio.h>

#define BUFF_MAX 128

LOCK_DEF

// implementar funcs variaveis para aceitar e mandar comandos
// criar struct packet maybe?

// type of AT commands
typedef enum AT_t {
	AT_TEST,
	AT_QUERY,
	AT_SET,
	AT_EXE,
}AT_t;

char* AT_t_to_string(AT_t type){
	switch (type) {
		case AT_TEST:
			return 
			}
}

base_t AT_init(){
	INIT_NCHECK();
	LOCK_INIT();
    UART_Initialize(115200);
	return SUCCESS;
}

base_t AT_Send(AT_t CMD_type,char* cmd,...){
	va_list list;
	va_start(list, cmd);
	char buf[BUFF_MAX];
	switch (CMD_type)
	{
		case AT_TEST:
			snprintf(buf, BUFF_MAX, "AT+%s=?",cmd);
			break;
		case AT_QUERY:
			snprintf(buf, BUFF_MAX, "AT+%s?",cmd);
			break;
		case AT_SET:
			snprintf(buf, BUFF_MAX, "AT+%s=",cmd);
			
			break;
		case AT_EXE:
			snprintf(buf, BUFF_MAX, "AT+%s",cmd);
			break;
		default:
			return ARG_ERROR;
	}
	va_end(list);
}
