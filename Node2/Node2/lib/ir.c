/*
 * ir.c
 *
 * Created: 08.11.2017 11:31:14
 *  Author: Arild Stenset
 */ 

 #define F_CPU 16000000UL
 #include <stdio.h>
 #include <avr/io.h>
 #include <util/delay.h>

 #include "ir.h"
 #include "adc.h"
 #include "can.h"

 #define NUM_OF_SAMPLES 10
 #define TRIGGER_LIMIT	25

 int ir_triggered(){
	int sum = 0;
	int mean = 0;
	for(int i = 0; i < NUM_OF_SAMPLES; i++){
		sum += adc_read(IR_CHANNEL);
		_delay_us(200);
	}
	mean = (sum/NUM_OF_SAMPLES);
	if(mean < TRIGGER_LIMIT){
		return 1;
	}
	else{
		return 0;
	}
 }

 void ir_canSend(){
	if(ir_triggered()){
		can_message_t msg;
		msg.id = GAME_MESSAGE_ID;
		msg.data[0] = 1;
		msg.length = 1;
		can_sendMessage(msg);
		_delay_ms(10);
	}
 }