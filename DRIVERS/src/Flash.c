/**
* @file Flash.c
* @brief This source file implements the API for controlling the LCD
* @version 2
* @date 27 Nov 2025
* @author Grupo06
*
*/
#include "Flash.h"
#include "LPC17xx.h"



CMD_RESULTS FLASH_EraseSector(unsigned int sector){

	unsigned int command[5] = {0}, out[5] = {0};

	command[0] = IAP_PREP;
	command[1] = sector;
	command[2] = sector;
	iap_entry(command, out);
	
	if(out[0] != CMD_SUCCESS){
		return out[0];
	}

	command[0] = IAP_ERASE;
	command[1] = sector;
	command[2] = sector;
	command[3] = SystemCoreClock / 1000;
	iap_entry(command, out);

	return out[0];
}

CMD_RESULTS sizeConv(unsigned int size){
	switch (size) {
        case 256:
        case 512:
        case 1024:
        case 4096:
            return (int)size;
        default:
            return -1;
    }
}

//pag 634

/* Escreve o bloco de dados referenciado por srcAddr, de dimensão size bytes,
no endereço da flash referenciado por dstAddr. */
CMD_RESULTS FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size){

	unsigned long src = (unsigned long) srcAddr;
	unsigned long dst = (unsigned long) dstAddr;

	unsigned int command[5] = {0}, out[5] = {0};
	unsigned int cmdSize;

	if((cmdSize = sizeConv(size)) < 0){
		return COUNT_ERROR; 
	}
	

	command[0] = IAP_PREP;
	command[1] = SECTOR;
	command[2] = SECTOR;
	iap_entry(command,out);

	if(out[0] != CMD_SUCCESS){
		return out[0];
	}
	
	command[0] = IAP_COPY;
	command[1] = dst;
    command[2] = src;
	command[3] = cmdSize;
	command[4] = SystemCoreClock / 1000;
	iap_entry(command, out);

	return out[0];
}

/* Compara o conteúdo do bloco de dados referenciado por srcAddr, de dimensão size bytes,
com o conteúdo do bloco de dados referenciado por dstAddr. 
@warning The result may not be correct when the source or destination includes any
 of the first 64 bytes starting from address zero. The first 64 bytes can be
 re-mapped to RAM.
*/
CMD_RESULTS FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size){
	
	unsigned long src = (unsigned long) srcAddr;
	unsigned long dst = (unsigned long) dstAddr;

	unsigned int command[5] = {0}, out[5] = {0};

	command[0] = IAP_COMP;
	command[1] = dst;
	command[2] = src;
	command[3] = size;
	iap_entry(command, out);

	return out[0];

}
