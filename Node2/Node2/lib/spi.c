/*
 * spi.c
 *
 * Created: 11.10.2017 13:50:29
 *  Author: Arild
 */ 

#include "spi.h"
#include <avr/io.h>

void spi_masterInit(void){
	DDRB |= (1 << PB2) | (1 << PB1) | (1 << PB7) | (1 << PB0);	//Set MOSI and SCK as outputs, set also PB0 as output because 2560-MCU is master
	DDRB &= ~(1 << PB3);							// MISO Input
	SPCR |= (1 << SPE) | (1 << MSTR);				//Set MOSI and SCK output, all others input, SPIE-SPI interrupt enable, SPE-SPI Enable, MSTR-Master/Slave Select
}

uint8_t spi_masterTransmit(char data){
	SPDR = data;							//Start transmission
	while(!(SPSR & (1 << SPIF))){};			//Waits for a transfer to complete, SPIF is set when a serial transfer is complete. 
	return SPDR;
}