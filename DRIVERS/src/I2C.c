/**
* @file I2C.c
* @brief This source file implements the API for the I2C controller
* @version 1.1
* @date 27 Nov 2025
* @author Grupo06
*
*/
//pag 439 442
#include "LPC17xx.h"
#include "Delay.h"
#include "I2C.h"
#include <stdbool.h>

#define GPIO0 LPC_GPIO0
#define SC LPC_SC       // system control
#define PC_I2C0 7
#define SDA_PIN 27
#define SCL_PIN 28
#define PINSEL_FUNC 0b01
#define PINCON LPC_PINCON
#define I2C LPC_I2C0

//--- I2CONCLR ---
#define AAC     1 << 2 // Assert acknowledge Clear bit
#define SIC     1 << 3 // I2C interrupt Clear bit
#define STAC    1 << 5 // START flag Clear bit
#define I2ENC   1 << 6 // I2C interface Disable bit
//--- I2CONSET ---
#define AA      1 << 2 // Assert acknowledge flag
#define SI      1 << 3 // I2C interrupt flag
#define STO     1 << 4 // STOP flag
#define STA     1 << 5 // START flag
#define I2EN    1 << 6 // I2C interface enable

#define I2CSTAT_MASK 0xF8

#define RADIO_ADDR 0x11                     // I2C Radio Address
#define I2CMW 0                             // I2C Master Write 
#define I2CMR 1                             // I2C Master Read




volatile I2C_SM conn;
volatile bool blocking;

void I2C0_IRQHandler(void){
    I2C_MState current_state = I2C->I2STAT & I2CSTAT_MASK;
    switch (current_state)
    {
    case Start_tx:
    case Start_Rtx:
        I2C->I2DAT = (conn.slave_addr << 1)  | conn.R_W;
        I2C->I2CONSET = AA;
        I2C->I2CONCLR = STAC;
        break;

    case TX_data:
        I2C->I2DAT = conn.data[conn.data_index++]; //  0 1 2 | 3
    case Request_RX:
        I2C->I2CONSET = AA;
        break;

    case RX_data_NACK:
        conn.data[conn.data_index++] = I2C->I2DAT;
    case Addr_NACK:
    case Request_RX_NACK:
    case TX_data_NACK:
        I2C->I2CONSET = STO | AA; 
        blocking = false;
        break;

    case TX_data_ACK:
        if (conn.data_index >= conn.data_size) // if last byte
        {
            I2C->I2CONSET = STO | AA;
            blocking = false; 
        }else{
            I2C->I2DAT = conn.data[conn.data_index++];
            I2C->I2CONSET = AA;
        }
        break;

    case Backed_off:
        I2C->I2CONSET = STA | AA ; 
        break;

    case RX_data:
        conn.data[conn.data_index++] = I2C->I2DAT; 
        if (conn.data_index >= conn.data_size) // if last byte
        {
            I2C->I2CONCLR = AAC;
            blocking = false;
        }else{
            I2C->I2CONSET = AA;
        }
        break;
    case 0xA0:       // State 0xA0: Slave Mode STOP received
        // We lost the bus or entered slave mode unexpectedly.
        // Force a STOP condition to reset the hardware logic.
        // I2C->I2CONCLR = AAC; 
        // I2C->I2CONSET = STO; 
        blocking = false;
        break;   
    default:
        //I2C->I2CONSET = STO; 
        blocking = false;    
        break;
    }
    conn.state = current_state;
    I2C->I2CONCLR = SIC;

}
 

void I2CMASTER_Init(void){
    SC->PCONP |= 1 << PC_I2C0; //1
    SC->PCLKSEL0 |= 00 << 14; // 2 PCLK = CCLK/4
    PINCON->PINSEL1 |=  PINSEL_FUNC << 22 | PINSEL_FUNC << 24; //3

    I2CMASTER_SetFrequency(f100k);
    NVIC_EnableIRQ(I2C0_IRQn);

    //__enable_irq();
    I2C->I2ADR0 = 0;      
    I2C->I2MASK0 = 0;
    I2C->I2CONCLR = AAC | SIC | STAC;
    I2C->I2CONSET = I2EN;

}

I2C_Freq I2CMASTER_SetFrequency(I2C_Freq frequency){

    int f;
    switch (frequency)
    {
    case f100k :
        f = 100000;
        break;
    case f400k:
        f = 400000;
        break;
    case f1M:
        f = 1000000;
        break;
    default:
        return Invalid_freq;
        break;
    }
    
    I2C->I2SCLH = I2C->I2SCLL = ((SystemCoreClock/4)/f)/2;
    return frequency;

    
}


I2C_MState I2CMASTER_Transmit(uint8_t devAddress, uint8_t *data, unsigned int size){
    conn = (I2C_SM){data,devAddress,I2CMW,size,0,None};
    I2C->I2CONCLR = SIC | STAC | AAC;   
    I2C->I2CONSET = STA;

    blocking = true; 
    // while (blocking)
    // {
    //     DELAY_Microseconds(100);
    // }
    while (blocking) {
        __WFI(); 
        DELAY_Microseconds(100);
    }
    return conn.state;
}

I2C_MState I2CMASTER_Receive(uint8_t devAddress, uint8_t *data, unsigned int size){
    conn = (I2C_SM){data,devAddress,I2CMR,size,0,None};
    I2C->I2CONCLR = SIC | STAC | AAC;   
    I2C->I2CONSET = STA;
    blocking = true;
    while (blocking) {
        __WFI();
        DELAY_Microseconds(100);
    }
    return conn.state;
}
