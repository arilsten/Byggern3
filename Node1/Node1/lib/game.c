/*
 * game.c
 *
 * Created: 19.11.2017 21:17:53
 *  Author: arilsten
 */ 

#define F_CPU 4915200UL
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "oled.h"
#include "game.h"
#include "can.h"
#include "menu.h"
#include "adc.h"
#include "highscore.h"
#include "motor.h"

#define INCREMENT_POINT	0x00
#define LIFE_LOST		0x01
#define JOYSTICK		0x00
#define SLIDER			0x01

void game_fullGameTraining(int mode);
void game_fullGameComp(int mode);

void game_fullGame(int mode){
	can_message_t msg;
	msg.id=GAME_MODE_ID;
	if (mode==MENU_COMP_SLIDE || mode==MENU_COMP_STICK){
		//name(); //Suddenly stopped working
		
		if(mode == MENU_COMP_SLIDE){
			msg.data[0]=SLIDER;
		}
		else{
			msg.data[0]=JOYSTICK;
		}
		msg.length=1;
		can_sendMessage(msg);	//Send can message som sier om vi vil ha slider eller joystick
		
		game_fullGameComp(mode);
	}
	else if (mode==MENU_TRA_SLIDE || mode==MENU_TRA_STICK){
		if (mode == MENU_TRA_SLIDE){
			msg.data[0]=SLIDER;
		} 
		else{
			msg.data[0]=JOYSTICK;
		}
		msg.length=1;
		can_sendMessage(msg);	
		
		game_fullGameTraining(mode);
	}
}


void game_fullGameTraining(int mode){
	int counter = 60;
	can_message_t msg;
	oled_reset();
	while (counter){
		if(can_getMessageReceived()){
			msg=can_read();
			can_clearMessageReceived();
			if(msg.id == GAME_MESSAGE_ID){
				if (msg.data[0]==INCREMENT_POINT){
					counter -= 1;
				}
			}
		}
		_delay_ms(20);
		motor_runAllDrivers(mode);
		oled_reset();
		oled_printGameTraining(counter);
	}
	oled_printGameOver();
	_delay_ms(500);
	while(!adc_isButtonPressed());
	
}

void game_fullGameComp(int mode){
	int score = 0;
	int lives = 3;
	int counter = 3;
	can_message_t msg;
	oled_reset();
	
	while(counter){ 
		if(can_getMessageReceived()){
			msg=can_read();
			can_clearMessageReceived(); 
			if(msg.id == GAME_MESSAGE_ID){
				if (msg.data[0]==INCREMENT_POINT){
					counter -= 1;
					oled_reset();
					oled_writeFromStartOnLine(3);
					oled_writeString("Sec to start:  ",14,0);
					oled_printNumber(counter);
				}
			}
		}
	} 
	
	while (1){
		if(can_getMessageReceived()){
			msg=can_read();
			can_clearMessageReceived();
			if(msg.id == GAME_MESSAGE_ID){
				if (msg.data[0]==INCREMENT_POINT){
					score += 1;
				}
				else{
					lives-=1;
					if (lives==0){
						break;
					}
					counter=3;
					oled_reset();
					oled_writeFromStartOnLine(3);
					oled_printNumber(counter);
					while (counter){
						_delay_ms(20);
						if(can_getMessageReceived()){
							msg=can_read();
							can_print(msg);
							if(msg.id == GAME_MESSAGE_ID){
								if (msg.data[0]==INCREMENT_POINT){
									counter-=1;
									oled_reset();
									oled_writeFromStartOnLine(3);
									oled_printNumber(counter);
								}
							}
						}
						motor_runAllDrivers(mode);
					}
				}
			}
		}
		_delay_ms(20);
		motor_runAllDrivers(mode);
		oled_reset();
		oled_printGame(score,lives);
	}
	motor_stopMotor();
	oled_printGameOver();
	_delay_ms(500);
	while(!adc_isButtonPressed());
	highscore_print(highscore_update(score));
	_delay_ms(500);
	while(!adc_isButtonPressed());
}


