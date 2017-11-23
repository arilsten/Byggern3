/*
 * motor.c
 *
 * Created: 13.11.2017 21:31:47
 *  Author: arilsten
 */ 

#define F_CPU 4915200UL
#include <util/delay.h>

#include "motor.h"
#include "can.h"
#include "menu.h"
#include "adc.h"


#include <stdio.h>

void motor_servoDriver();			//Sends can message with the desired servo position
void motor_joystickMotorDriver();	//Sends can message with the desired motor speed read from joystick
void motor_sliderMotorDriver();		//Sends can message with the desired motor position read from slider
void motor_solenoidDriver();		//Sends can message to trigger the solenoid

void motor_servoDriver(){
	can_message_t servoData;
	servoData.id = SERVO_ID;
	servoData.data[0] = 0;
	servoData.data[1] = adc_readLeft();
	servoData.length = 2;
	can_sendMessage(servoData);
}

void motor_joystickMotorDriver(){
	can_message_t motorData;
	motorData.id = MOTOR_ID;
	motorData.data[0] = adc_readX();
	motorData.data[1] = 0;
	motorData.length = 2;
	can_sendMessage(motorData);
}

void motor_sliderMotorDriver(){
	can_message_t motorData;
	motorData.id = MOTOR_ID;
	motorData.data[0] = adc_readRight();
	motorData.data[1] = 0;
	motorData.length = 2;
	can_sendMessage(motorData);
}


void motor_solenoidDriver(){
	can_message_t solenoidData;
	solenoidData.id = SOLENOID_ID;
	solenoidData.data[0] = 1;
	solenoidData.length = 1;
	can_sendMessage(solenoidData);
}

void motor_runAllDrivers(int mode){
	motor_servoDriver();
	_delay_ms(20);
	
	if (mode == MENU_COMP_SLIDE || mode == MENU_TRA_SLIDE){
		motor_sliderMotorDriver();
	}
	else if (mode == MENU_COMP_STICK || mode == MENU_TRA_STICK){
		motor_joystickMotorDriver();
	}	
	_delay_ms(20);
	
	if (adc_isButtonPressed()){
		motor_solenoidDriver();
		_delay_ms(20);
	}
}

void motor_stopMotor(){
	can_message_t motorData;
	motorData.id = MOTOR_ID;
	motorData.data[0] = 255/2;
	motorData.data[1] = 0;
	motorData.length = 2;
	can_sendMessage(motorData);	
}
	