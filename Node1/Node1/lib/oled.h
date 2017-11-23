/*
 * oled.h
 *
 * Created: 27.09.2017 17:22:20
 *  Author: eddas
 */ 

#include <avr/io.h>

#ifndef OLED_H_
#define OLED_H_


void oled_init();
void oled_reset();
void oled_clearLine(uint8_t line);
void oled_bright();
void oled_writeCommand(uint8_t command);
void oled_writeChar(char* str);
void oled_writeCharInv(char* str);
void oled_writeString(char* str, int size, int inv);
void oled_writeFromStartOnLine(int line);
void oled_writePic();
void oled_fillSram();
void oled_printGame(int score,int lives);
void oled_printGameTraining(int counter);
void oled_printNumber(int number);
void oled_printNumberInverted(int number);
void oled_printGameOver();
void oled_writeLongerText(int startLine, char* text);
void oled_printStartPage();


#endif /* OLED_H_ */

