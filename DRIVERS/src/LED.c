/**
* @file LED.c
* @brief This source file implements the API for controlling the LED
* @version 1.1
* @date 07 Out 2025
* @author Grupo06
*
*/

#include "LED.h"
#define LED_PIN 22
#define LED 1 << LED_PIN
#define GPIO0 LPC_GPIO0
#define SC LPC_SC// system control

static int led_state;
/* Faz a iniciação do sistema para permitir a manipulação do estado LED do sistema de
* protopagem (LPCXPRESSO LPC1769), onde o parâmetro state se igual a false o LED
* fica apagado ou caso contrario fica aceso. */
void LED_Init(bool state){
	SC->PCONP |= 1 << 15; // PCGPIO Power/clock control bit for IOCON, GPIO, and GPIO interrupts
	GPIO0->FIODIR |= LED;
	GPIO0->FIOSET = state ? LED : 0;
	led_state = state;
}
/* Devolve true se o LED está aceso e false se o LED está apagado. */
bool LED_GetState(void){
	return led_state;
}
/* Acende o LED. */
void LED_On(void){
	GPIO0->FIOSET = LED;
	led_state = 1;
}
/* Apaga o LED. */
void LED_Off(void){
	GPIO0->FIOCLR = LED;
	led_state = 0;
}
/* Troca o estado do LED. Se estiver aceso apaga o LED, caso contrario acende o LED. */
void LED_Toggle(void){
	led_state ? LED_Off() : LED_On();
}
