/*
 * adc.c
 *
 * Created: 08.11.2017 13:41:06
 *  Author: Arild Stenset
 */ 

 #include <avr/io.h>
 #include "adc.h"

 void adc_init(void){
	DDRF &= ~(1 << PF0);													// Pin as input
	ADMUX |= (1 << REFS0);													// Referansespenning
	ADMUX &= ~(1 << REFS1);													// Referansespenning
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);		// ADEN - ADC Enabled, ADPS2:0 - Prescaler
 }

 uint8_t adc_read(uint8_t channel){
	ADMUX |= ((1 << channel) & 0xE0);
	ADCSRA |= (1 << ADSC);			// ADC Start conversion
	while((ADCSRA & (1<<ADSC))){
		//Do nothing
	}
	return ADC;
 }