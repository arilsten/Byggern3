/*
 * uart.c
 *
 * Created: 27.09.2017 17:21:39
 *  Author: eddas
 */ 
#include "uart.h"
#include <avr/io.h>


void uart_init(unsigned int ubrr){
	
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	
	//Enable receiver and transmitter:
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);

	//Set format: 1 stop bit, 8 data bits.
	UCSR0C |= (1 << USBS0) | (1 << UCSZ00) | (1 << UCSZ01);
		
}

void uart_transmit(char c) {
	//Sjekk om det er klart for å sende, hvis ikke, vent til det er klart
	//Skriv riktig bokstav til riktig register
	
	//Wait for empty transmit buffer
	while(!(UCSR0A & (1<<UDRE0))){}

	//Put data into buffer;
	UDR0 = c;
}

unsigned char uart_receive(){
	while (!(UCSR0A & (1 << RXC0))) {}
	
	return UDR0;
}
