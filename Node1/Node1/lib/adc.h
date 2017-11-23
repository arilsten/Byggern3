/*
 * adc.h
 *
 * Created: 27.09.2017 17:20:53
 *  Author: eddas
 */ 

//#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef ADC_H_
#define ADC_H_

ISR(INT2_vect);
void adc_interruptInit();	
int adc_isButtonPressed();	
int adc_readX();			//Reads the x position of the joystick
int adc_readY();			//Reads the y position of the joystick
int adc_readLeft();			//Reads the position of the right slider
int adc_readRight();		//Reads the position of the right slider

#endif /* ADC_H_ */