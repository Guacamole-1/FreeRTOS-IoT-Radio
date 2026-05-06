#ifndef MENU_RTOS_H_
#define MENU_RTOS_H_

#include "base.h"
#include "Radio.h"
#include "FreeRTOS.h"
#include "task.h"



/**
 * Inicializa o menu RTOS.
 * Recebe uma cópia inicial de Radio_Data. Pode ser NULL, mas o ideal é passar
 * a mesma estrutura usada na inicialização do rádio.
 */
base_t MENU_RTOS_Init(Radio_Data *initial_data);

/**
 * Cria a task principal do menu.
 * O DISPLAY_Manager() deve continuar a correr noutra task, porque este módulo
 * apenas envia comandos para a queue do display.
 */
base_t MENU_RTOS_Start(UBaseType_t priority, uint16_t stack_size);

/** Atualiza a cópia local do estado do rádio usada pelo menu. */
base_t MENU_RTOS_SetRadioData(const Radio_Data *data);

/** Lê a cópia local do estado do rádio usada pelo menu. */
base_t MENU_RTOS_GetRadioData(Radio_Data *data);



#endif /* MENU_RTOS_H_ */
