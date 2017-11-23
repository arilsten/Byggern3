/*
 * max520.h
 *
 * Created: 11.11.2017 14:43:19
 *  Author: Arild Stenset
 */ 


#ifndef MAX520_H_
#define MAX520_H_

#include <avr/interrupt.h>
#include <stdint.h>

void max520_init(void);
void max520_send(uint8_t val);

#endif /* MAX520_H_ */