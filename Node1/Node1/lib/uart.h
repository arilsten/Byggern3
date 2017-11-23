/*
 * uart.h
 *
 * Created: 27.09.2017 17:21:53
 *  Author: eddas
 */ 


#ifndef UART_H_
#define UART_H_
#define UBRR 31
#include <avr/io.h>

void uart_init(unsigned int baud);
void uart_transmit(char c);
unsigned char uart_receive();

#endif /* UART_H_ */