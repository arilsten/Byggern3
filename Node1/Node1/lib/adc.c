/*
 * adc.c
 *
 * Created: 27.09.2017 17:20:20
 *  Author: eddas, robbas og arras
 */ 

#define F_CPU 4915200UL
#include <util/delay.h>
#include <avr/interrupt.h>

#include "adc.h"

volatile char *ADC_ptr = (char *) 0x1400;
volatile int interrupt_triggered = 0;

ISR(INT2_vect){
	interrupt_triggered = 1;
}

void adc_interruptInit(){
	GICR |= (1<<INT2);					// INT2 belongs to adc.
	EMCUCR &= ~(1 << ISC2);				// Interrupt on falling edge for adc, page 85 in datasheet for Atmega 162.
}

int adc_isButtonPressed(){
	if(!(PINB & (1 << PINB1))){
		return 1;
	}
	return 0;
}

int adc_readX(){
	ADC_ptr[0] = 0x05;
	
	while(interrupt_triggered == 0){}
	interrupt_triggered = 0;
	
	int x_value = ADC_ptr[0];
	_delay_ms(20);
	return (x_value);
}

int adc_readY(){
	ADC_ptr[0]=0x04;
	
	while(interrupt_triggered==0){	}
	interrupt_triggered = 0;
	
	int y_value=ADC_ptr[0];
	_delay_ms(20);
	return (y_value);
}

int adc_readLeft(){
	ADC_ptr[0]=0x06;
	
	while(interrupt_triggered==0){	}
	interrupt_triggered = 0;
	
	int l_value=ADC_ptr[0];
	_delay_ms(20);
	return (l_value);
}

int adc_readRight(){
	ADC_ptr[0]=0x07;
	
	while(interrupt_triggered==0){	}
	interrupt_triggered = 0;
	
	int r_value=ADC_ptr[0];
	_delay_ms(20);
	return (r_value);
}