/*
 * game.c
 *
 * Created: 08.11.2017 11:34:25
 *  Author: Arild Stenset
 */ 

#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>

#include "game.h"
#include "adc.h"
#include "can.h"
#include "ir.h"
#include "pwm.h"
#include "motor.h"
#include "solenoid.h"

volatile uint8_t tot_overflow;
volatile int sendGameCounter = 0;
volatile int can_message_received = 0;

 void game_sendGameTimer(void){
	if(sendGameCounter){
		cli();
		sendGameCounter = 0;
		can_message_t msg;
		msg.id = GAME_MESSAGE_ID;
		msg.data[0] = 0;
		msg.data[1] = 0;
		msg.length = 2;
		can_sendMessage(msg);
		sei();
	}
 }
 
 void game_timerInit(void){				// initialize game-timer, interrupt causes a can message being sent if the ir sensor has been triggered.
	 TCCR1B |= (1 << CS11);				// set up timer with prescaler = 8
	 TCNT1 = 0;							// initialize counter
	 TIMSK1 |= (1 << TOIE1);			// enable overflow interrupt
	 tot_overflow = 0;					// initialize overflow counter variable
 }

 void game_runGame(void){
	while(1){
		if(can_message_received){
			printf("Can message received: %d\n", can_message_received);
			cli();
			can_message_received = 0;
			can_message_t canMsg = can_read();

			switch (canMsg.id){
					case SERVO_ID:
					pwm_setServo(canMsg);
					//printf("Servo set to %d\n", canMsg.data[1]);
					sei();
					break;
				case MOTOR_ID:
					
					//motor_driver(canMsg);
					//motor_speedReg(canMsg);
					motor_posReg(canMsg);

					//printf("Motor set to %d\n", canMsg.data[0]);
					sei();
					break;
				case SOLENOID_ID:
					if(canMsg.data[0]){		//data[0] represents if solenoid should hit or not
						solenoid_enable();
						//printf("Solenoid activated\n");
					}
					sei();
					break;
				
				default:
					printf("Default \n");
					sei();
					break;
				}

		}
		ir_canSend();
		game_sendGameTimer();
		_delay_ms(10);
	}
}
 
 ISR(TIMER1_OVF_vect){
	 tot_overflow++;
	 if (tot_overflow >= 31){			// 31 overflows = 1 seconds delay (approx.)
		sendGameCounter = 1;
		tot_overflow = 0;
	 }
 }

 ISR(INT2_vect){								// Can interrupt
	 can_message_received = 1;
 }
 
