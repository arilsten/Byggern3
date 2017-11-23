/*
 * solenoid.c
 *
 * Created: 12.11.2017 17:23:05
 *  Author: Arild Stenset
 */ 

 #define F_CPU 16000000UL
 #include <util/delay.h>
 #include <avr/io.h>
 #include "solenoid.h"

 void solenoid_init(){
	DDRL |= (1 << PL6);						// PIN 4 on atmega2560 as solenoid output
	PORTL &= ~(1 << PL6);					// Disable relay on PIN 43, not 41
 }

 void solenoid_enable(){
		PORTL |= (1 << PL6);			// Enable relay on PIN 43, not 41 on the atmega2560
		_delay_ms(20);					// Hold relay pin active
		PORTL &= ~(1 << PL6);			// Disable relay on PIN 43, not 41 on the atmega2560
 }