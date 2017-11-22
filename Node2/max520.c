/*
 * max520.c
 *
 * Created: 11.11.2017 14:43:03
 *  Author: Arild Stenset
 */ 

 #include "max520.h"
 #include "TWI_Master.h"
 #include <avr/interrupt.h>
 #include <stdint.h>

 #define MAX520_ADDR 0x50
 
 void max520_init(void){
	 //TWI_Master_Initialise();
	 sei();
 }

 void max520_send(uint8_t data){
	 unsigned char msg[3] = {MAX520_ADDR, 0, data}; //hardcoded, address and ch could be parametres of function
	 TWI_Start_Transceiver_With_Data(msg, 3);
 }
 