/*
 * uart.c
 *
 * Created: 27.09.2017 17:21:39
 *  Author: eddas
 */ 
#include "uart.h"
#include <avr/io.h>

#define F_CPU 4915200UL
#define BAUD 9600

void uart_init(unsigned int baud){
	UBRR0H = (unsigned char)(UBRR >> 8);
	UBRR0L = (unsigned char) UBRR;
	
	//Enable receiver and transmitter:
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}

void uart_transmit(char c) {
	
	//Wait for empty transmit buffer
	while(!(UCSR0A & (1<<UDRE0))){}

	//Put data into buffer;
	UDR0 = c;
}

unsigned char uart_receive(){
	while (!(UCSR0A & (1 << RXC0))) {}
	
	return UDR0;
}
