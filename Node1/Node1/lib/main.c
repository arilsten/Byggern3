#define F_CPU 4915200UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "init.h"
#include "uart.h"
#include "adc.h"
#include "oled.h"
#include "sram.h"
#include "menu.h"
#include "MCP2515.h"
#include "spi.h"
#include "can.h"
#include "game.h"
#include "highscore.h"


volatile int new_can_message_received = 0;


void pingPong(){
	oled_fillSram();
	_delay_ms(100);
	oled_writePic();
	_delay_ms(500);
	
	while(!adc_isButtonPressed());	
	int id;
	//id=menu_funct();
	id = MENU_COMP_STICK;	//menu_funct suddenly stopped working so we set the id like this instead
	game_fullGame(id);
}



int main(){
	initialization();
	highscore_init();
	fdevopen(&uart_transmit, &uart_receive);
	printf("reset \t");
	sei();
	while(1){			
		pingPong();
	}

	return 0;
}

ISR(INT0_vect){						
	can_setMessageReceived();
}