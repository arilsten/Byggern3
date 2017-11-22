/*
 * byggern2560.c
 *
 * Created: 01.11.2017 13:06:58
 * Author : Arild Stenset
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>

#include "uart.h"
#include "init.h"
#include "game.h"

// INT2 - CAN message received
// TIMER5_OVF_vect - Used for calculating dt in the regulator
// TIMER1_OVF_vect - SendGameStatus every 1 sec

int main(void){
	initialize();
	fdevopen(&uart_transmit, &uart_receive);
	sei();

	while(1){
		game_runGame();
	
	}
	
    return 0;
}