/*
 * sram.h
 *
 * Created: 27.09.2017 17:21:22
 *  Author: eddas
 */ 

#include <avr/io.h>

#ifndef SRAM_H_
#define SRAM_H_

#define length_of_name			0x0010
#define length_of_score			0x0001
#define number_of_highscores	0x0004

#define CURRENTNAME				0x0000
#define HIGHSCORENAME1			CURRENTNAME+length_of_name			//0x0000 til 0x000F altså 16 tegn 
#define HIGHSCORENAME2			HIGHSCORE1+length_of_name
#define HIGHSCORENAME3			HIGHSCORE2+length_of_name
#define HIGHSCORENAME4			HIGHSCORE3+length_of_name

#define CURRENTSCORE			0x0100
#define HIGHSCORE1				CURRENTSCORE+length_of_score
#define HIGHSCORE2				HIGHSCORE1+length_of_score
#define HIGHSCORE3				HIGHSCORE2+length_of_score
#define HIGHSCORE4				HIGHSCORE3+length_of_score

void SRAM_test(void);
char sram_getInfo(uint16_t address);
void sram_setInfo(uint16_t address, uint8_t value);
void sram_getName(uint16_t address,char* name);
int sram_getScore(uint16_t address);
void sram_setName(uint16_t address,char* name);
void sram_setScore(uint16_t address,int score);

#endif /* SRAM_H_ */