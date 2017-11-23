/*
 * spi.c
 *
 * Created: 11.10.2017 13:50:29
 *  Author: eddas
 */ 

#include "spi.h"
#include <avr/io.h>

void SPI_masterInit(void){
	DDRB |= (1 << PB5) | (1 << PB7);	//Set MOSI and SCK as outputs
	DDRB &= ~(1 << PB6);				// MISO Input
	SPCR |= (1 << SPE) | (1 << MSTR);	//Set MOSI and SCK output, all others input, SPIE-SPI interrupt enable, SPE-SPI Enable, MSTR-Master/Slave Select
}

uint8_t SPI_masterTransmit(char data){
	
	SPDR = data;						//Start transmission
	
	while(!(SPSR & (1 << SPIF)));		//Waits for a transfer to complete, SPIF is set when a serial transfer is complete. 
	
	return SPDR;
	
}