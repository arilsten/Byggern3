/*
 * Highscore.c
 *
 * Created: 20.11.2017 09:22:09
 *  Author: eddas
 */ 
#include "adc.h"
#include "oled.h"
#include "sram.h"
#include "highscore.h"
#include "menu.h"

#define F_CPU 4915200UL
#include <util/delay.h>
#include <stdio.h>




void highscore_print(int currentPlace){ //Current place showes which highscore is new. 0 if there is no new highscore
	oled_reset();
	oled_writeFromStartOnLine(0);
	oled_writeString(" Highscore   ",11,0);
	char name[length_of_name];
	char number[3];
	number[1] = ':';
	number[2] = ' ';
	int inverted=0;
	for(int i = 0; i < number_of_highscores; i++){
		if (i+1==currentPlace){
			inverted=1;
		}
		else{
			inverted=0;
		}
		oled_writeFromStartOnLine(i+1);
		number[0]=(char) (i+49);
		oled_writeString(number,3,inverted);
		sram_getName(HIGHSCORENAME1+i*length_of_name,name);
		oled_writeString(name,length_of_name,inverted);
		oled_writeString("  ",1,inverted);

		if (inverted){
			oled_printNumberInverted(sram_getScore(HIGHSCORE1 + i*length_of_score));
		}
		else{
			oled_printNumber(sram_getScore(HIGHSCORE1 + i*length_of_score));
		}
	}
}

void highscore_init(){
	char empty[length_of_name];
	for (int i = 0; i < length_of_name; i++){
		empty[i]=' ';
	}
	for (int i = 0; i < number_of_highscores; i++){
		sram_setScore(HIGHSCORE1+i*length_of_score,0);
		sram_setName(HIGHSCORENAME1+i*length_of_name,empty);
	}
	sram_setName(CURRENTNAME,"No name         ");
}

int highscore_update(int currentScore){
	char name[length_of_name];
	for (int i = 0; i < number_of_highscores; i++){
		if (currentScore > sram_getScore(HIGHSCORE1+i*length_of_score) ){
			
			for(int j = number_of_highscores-1; j > i  ; j--){
				sram_setScore(HIGHSCORE1+j*length_of_score,sram_getScore(HIGHSCORE1+(j-1)*length_of_score));
				sram_getName(HIGHSCORENAME1+(j-1)*length_of_name,name);
				sram_setName(HIGHSCORENAME1+j*length_of_name,name);
			}
			
			sram_setScore(HIGHSCORE1+i*length_of_score,currentScore);
			sram_getName(CURRENTNAME,name);
			sram_setName(HIGHSCORENAME1+i*length_of_name,name);
			return i+1;
		}
	}
	return 0;
}