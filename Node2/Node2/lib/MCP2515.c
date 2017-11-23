/*
 * mcp2515.c
 *
 * Created: 11.10.2017 15:49:39
 *  Author: eddas, arild og robin
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include "MCP2515.h"
#include "spi.h"
#include <avr/io.h>
#include <stdio.h>

void mcp2515_init(void){
	mcp2515_reset();
	_delay_ms(10);
}

uint8_t mcp2515_read(uint8_t address){
	uint8_t result;
	PORTB &= ~(1 << PB7);	//Lower the CS pin of the can controller
	spi_masterTransmit(MCP_READ);	//Read
	spi_masterTransmit(address);
	result = spi_masterTransmit(address);	//Sends "address" just to send something. Edda is don't care.
	PORTB |= (1 << PB7);	//Put the CS pin high
	return result;
}

void mcp2515_write(uint8_t address, uint8_t data){
	PORTB &= ~(1 << PB7);	//Lower the CS pin of the can controller
	spi_masterTransmit(MCP_WRITE);	//Write
	spi_masterTransmit(address);
	spi_masterTransmit(data);
	PORTB |= (1 << PB7);	//Put the CS pin high
}

void mcp2515_RTS(){
	PORTB &= ~(1 << PB7);	//Lower the CS pin of the can controller
	spi_masterTransmit(MCP_RTS_TX0);
	PORTB |= (1 << PB7);	//Put the CS pin high
}

uint8_t mcp2515_readStatus(){
	uint8_t status;
	PORTB &= ~(1 << PB7);	//Lower the CS pin of the can controller
	spi_masterTransmit(MCP_READ_STATUS);
	status = spi_masterTransmit(0x00);	//0x00 = send some don't care data, to receive data from the can-controlleren.
	PORTB |= (1 << PB7);	//Put the CS pin high
	return status;
}

void mcp2515_bitModify(uint8_t address, uint8_t maskbyte, uint8_t databyte){
	PORTB &= ~(1 << PB7);	//Lower the CS pin of the can controller
	spi_masterTransmit(MCP_BITMOD);	//Bit Modify
	spi_masterTransmit(address);
	spi_masterTransmit(maskbyte);	//A '1' in the mask register will allow a bit in the register to change, while a '0' will not.
	spi_masterTransmit(databyte);	//A ‘1’ in the data byte will set the bit and a ‘0’ will clear the bit
	PORTB |= (1 << PB7);	//Put the CS pin high
}

void mcp2515_reset(){
	PORTB &= ~(1 << PB7);	//Lower the CS pin of the can controller
	spi_masterTransmit(MCP_RESET);
	PORTB |= (1 << PB7);	//Put the CS pin high
	
}

void mcp2515_printMode(){
	uint8_t mode;
	mode = mcp2515_read(MCP_CANSTAT);
	mode &= 0xE0;
	if(mode == MODE_NORMAL){
		printf("Normal mode\n");
	}
	else if(mode == MODE_LOOPBACK){
		printf("Loopback mode\n");
	}
	else if(mode == MODE_CONFIG){
		printf("Config mode\n");
	}
	else{
		printf("Unknown Mode = %d\n", mode);
	}
}