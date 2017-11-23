/*
 * init.c
 *
 * Created: 07.11.2017 18:36:42
 *  Author: arilsten
 */ 

#include <avr/io.h>
#include <stdio.h>

#include "init.h"
#include "uart.h"
#include "adc.h"
#include "oled.h"
#include "sram.h"
#include "menu.h"
#include "MCP2515.h"
#include "spi.h"
#include "can.h"
#include "highscore.h"

void initialization(void){
	SFIOR |= (1 << XMM2);			// This has to be set in order to use the programming pins for JTAG as normal, it then follows that a 64kbit address space cannot be reached.
	MCUCR |= (1 << SRE);			// Enable External SRAM
	DDRE |= (1 << PE1);				// Puts PE1 high because the latch is active low.
	DDRB &= ~(1 << PB1);			// Sets joystick pushbutton as input.
	
	uart_init(UBRR);
	oled_init();
	adc_interruptInit();			// Initialize adc-interrupt
	SPI_masterInit();
	mcp2515_init();
	can_init();
}