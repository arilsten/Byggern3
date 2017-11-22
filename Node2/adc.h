/*
 * adc.h
 *
 * Created: 08.11.2017 13:41:26
 *  Author: Arild Stenset
 */ 

 #include <stdio.h>

#define IR_CHANNEL 0			// A0, Analog0

#ifndef ADC_H_
#define ADC_H_

void adc_init(void);
uint8_t adc_read(uint8_t channel);

#endif /* ADC_H_ */