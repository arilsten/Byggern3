/*
 * pwmDriver.c
 *
 * Created: 01.11.2017 16:00:38
 *  Author: Arild Stenset
 */ 

 #define F_CPU 16000000UL
 #include <util/delay.h>
 #include <avr/io.h>
 #include <stdio.h>
 #include "pwm.h"
 #include "can.h"

 void pwm_init(void){
	DDRE |= (1 << PE3);				// Pin #5 on the 2560 is pwm output

	//fast pwm
	TCCR3A &= ~(1<< WGM30);
	TCCR3A |= (1<<WGM31);
	TCCR3B |= (1<<WGM32) | (1<<WGM33);
	
	//non-inverting mode
	TCCR3A &= ~(1<<COM3A0);			// Clear OCnA/OCnB/OCnC on compare match, set OCnA/OCnB/OCnC at BOTTOM (non-inverting mode)
	TCCR3A |= (1<<COM3A1);			// -------------------------||-----------------------------
	
	//prescaler at 1024
	TCCR3B |= (1<<CS30) | (1<<CS32);
	TCCR3B &= ~(1<<CS31);
	
	ICR3 = 312;
	
	pwm_setValue(PWM_MID);			//center servo on init
}

void pwm_setValue(uint8_t value){
	OCR3A = value;
}

void pwm_test(){
	pwm_setValue(PWM_MIN);
	_delay_ms(1000);
	pwm_setValue(PWM_MAX);
	_delay_ms(1000);
}

void pwm_setServo(can_message_t msg){
	if (msg.id == SERVO_ID){									// servo.id = 0x0013
		uint8_t value = (((msg.data[1]*16)/255) + PWM_MIN);		// Scaling of input from joystick

		if((value <= PWM_MID+2) && (value >= PWM_MID-2)){
			pwm_setValue(PWM_MID);
		}
		else if((value <= PWM_MAX) && (value >= PWM_MIN)){
			pwm_setValue(value);
		}
		else{
			pwm_setValue(PWM_MID);
			printf("ELSE\n");
		}
	}
}