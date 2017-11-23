/*
 * menu.c
 *
 * Created: 04.10.2017 16:39:11
 *  Author: eddas
 */ 

#include "adc.h"
#include "oled.h"
#include "menu.h"
#include "sram.h"

#include <string.h>

#define F_CPU 4915200UL
#include <util/delay.h>
#include <stdio.h>

#define number_of_pages 1
#define menu_items 3

const int number_of_lines = 8;
const int length=17;
char back[20]="back                ";
char player1[20];
int line_to_invert=0;




int menu_funct(){
	printf("Mf starT");
	static menu* allmenu=NULL;
	if(allmenu==NULL){
		printf("IF");
		allmenu=menu_createAllMenu();
		printf("IFdonne");
	}
	printf("intitDone\n");
	return menu_control(allmenu);
}


void menu_makeMenu(menu* newmenu, menu *_parent,menu **_children,int _numb_children,char* _name){
	newmenu->parent=_parent;
	for (int i = 0; i<_numb_children;i++){
		newmenu->children[i]=_children[i];
	}
	newmenu->numb_children=_numb_children;
	int i=0;
	while(_name[i]!='.'){
		newmenu->name[i]=_name[i];
		i+=1;
	}
	while(i<length){
		newmenu->name[i]=' ';
		i+=1;
	}
}

void menu_makeMenuOnlyName(menu* newmenu,char* _name){
	int i=0;
	while(_name[i]!='.'){
		newmenu->name[i]=_name[i];
		i+=1;
	}
	while(i<length){
		newmenu->name[i]=' ';
		i+=1;
	}
	newmenu->numb_children=0;
	newmenu->id=MENU_INVALIDE;
}

void menu_addChildParent(menu* _parent, menu* _child){
	_parent->children[_parent->numb_children]=_child;
	_parent->numb_children+=1;
	_child->parent=_parent;
}

void menu_addParentAsLastChild(menu* _menu){
	_menu->children[_menu->numb_children]=_menu->parent;
	_menu->numb_children+=1;
}

void menu_printInfo(int id){
	oled_reset();
	char* info;
	switch(id) {
		case MENU_COMP_INFO:
		oled_writeFromStartOnLine(0);
		oled_writeString("Competition Information  ",24,0);
		info="In competition mode your score get counted and you have a chance to reach the highscore. Therefor you have to enter your name in this mode \0";
		oled_writeLongerText(1,info);
		break;
		
		case MENU_TRA_INFO:
		oled_writeFromStartOnLine(0);
		oled_writeString("Training Mode Information  ",26,0);
		info="In training mode points don't get counted. One round last for 60 seconds :) \0";
		oled_writeLongerText(1,info);
		break;
		
		/* you can have any number of case statements */
		default: /* Optional */
		info="Upsi!! Something went wrong :( \0";
		oled_writeLongerText(1,info);

	}
}

int menu_control(menu* current_menu){
	oled_reset();
	if(current_menu->numb_children==0){
		if (current_menu->id >= 100){
			menu_printInfo(current_menu->id);
			_delay_ms(300);
			
			while(!adc_isButtonPressed());
			return menu_control(current_menu->parent);
		}
		else{
			return current_menu->id;
		}
		
	}
	
	oled_writeFromStartOnLine(0);//Printer overskrift
	oled_writeString(current_menu->name,length,0);
	printf("hei: \n");
	for (int i=0; i< current_menu->numb_children;i++){
		oled_writeFromStartOnLine(i+1);
		oled_writeString(current_menu->children[i]->name,length,0);//Printer alternativ
	}
	if(current_menu->children[current_menu->numb_children-1]==current_menu->parent){
		oled_writeFromStartOnLine(current_menu->numb_children);
		oled_writeString(back,length,0);//Legger til back som alternativ
	}

	oled_writeFromStartOnLine(1);
	oled_writeString(current_menu->children[0]->name,length,1);//Velger linje 1 som valgt ved start
	_delay_ms(500);
	int index=0;
	line_to_invert=0;
	while(1){
		index = menu_step(current_menu);
		if (adc_isButtonPressed()){
			printf("Button pressed\n");
			
			return menu_control(current_menu->children[index]);
		}
		_delay_ms(200);
	}
}

int menu_step(menu* _menu){
	int direction = 0;
	int joystick_Position = adc_readY()*0.784314-100;
	
	if(joystick_Position > 20){
		direction = 1;
	}

	if(joystick_Position < -20){
		direction = -1;
	}
	if(direction){
		oled_writeFromStartOnLine(line_to_invert+1);
		if (_menu->parent==_menu->children[line_to_invert]){
			oled_writeString(back,length,0);	
		}
		else{
			oled_writeString(_menu->children[line_to_invert]->name,length,0);
		}
		
		line_to_invert = (line_to_invert-direction+_menu->numb_children)%(_menu->numb_children);
		oled_writeFromStartOnLine(line_to_invert+1);
		if (_menu->parent==_menu->children[line_to_invert]){
			oled_writeString(back,length,1);
		}
		else{
			oled_writeString(_menu->children[line_to_invert]->name,length,1);
		}
	}
	return line_to_invert;
}

void menu_addSpaces(char* word,int startIndex,int size){
	for(int i = startIndex; i < size; i++){
		word[i]=' ';
	}
}

void menu_enterWord(char* question, int questionLength){
	const int numberOfLetters=30;
	const int lettersInAlphabet=26;
	char letters[numberOfLetters][length];
	
	for (int i=0;i<lettersInAlphabet;i++){	//Adding letters as alternatives
		letters[i][0]=(char) 65+i;
		menu_addSpaces(letters[i],1,length);
	}

	letters[lettersInAlphabet][0]=' '; //Adding space as alternativ
	menu_addSpaces(letters[lettersInAlphabet],1,length);

	strncpy(letters[lettersInAlphabet+1], "Backspace", length);
	menu_addSpaces(letters[lettersInAlphabet+1],9,length);
	
	strncpy(letters[lettersInAlphabet+2], "caps lock", length);
	menu_addSpaces(letters[lettersInAlphabet+2],9,length);
	

	strncpy(letters[lettersInAlphabet+3], "Done", length);	
	menu_addSpaces(letters[lettersInAlphabet+3],4,length);
	//Initialize array done
	
	char currentWord[length];
	menu_addSpaces(currentWord,0,length);
	int lengthOfCurrentWord=0;
	int currentIndex=0;
	int direction;
	int joystick_Position;
	int asciiOffset=65;
	line_to_invert=1;

	oled_reset();
	oled_writeFromStartOnLine(0);//Printer overskrift
	oled_writeString(question,questionLength,0);
	
	for (int i=1; i < number_of_lines; i++){
		oled_writeFromStartOnLine(i);
		oled_writeString(letters[i-1], length, 0);//Printer alternativ
	}
	oled_writeFromStartOnLine(line_to_invert);
	oled_writeString(letters[currentIndex], length, 1);//Velger linje 1 som valgt ved start
	
	while (1){
		_delay_ms(250);
		direction = 0;
		joystick_Position = (-255 + 2*adc_readY());
		
		if(joystick_Position > 60){
			direction = 1;
		}
		if(joystick_Position < -60){
			direction = -1;
		}
	
		
		if(direction){
			oled_writeFromStartOnLine(line_to_invert);
			oled_writeString(letters[currentIndex],length,0);	//Fjerner invertering på den som hadde det
	
			line_to_invert -= direction;
			currentIndex -= direction;
			
			if(currentIndex<0){							//På toppen av menyen og ønsker på opp
				currentIndex=numberOfLetters-1;
				line_to_invert=number_of_lines-1;
				for(int i = 1; i < line_to_invert; i++){
					oled_writeFromStartOnLine(i);
					oled_writeString(letters[currentIndex-line_to_invert+i],length,0);
				}
				oled_writeFromStartOnLine(line_to_invert);
				oled_writeString(letters[currentIndex],length,1);
			}
			else if (currentIndex>=numberOfLetters){	//På bunnen av meny, ønsker gå ned
				currentIndex=0;
				line_to_invert=1;
				for(int i = 1; i < number_of_lines-1; i++){
					oled_writeFromStartOnLine(i+line_to_invert);
					oled_writeString(letters[currentIndex+i],length,0);
				}
				oled_writeFromStartOnLine(line_to_invert);
				oled_writeString(letters[currentIndex],length,1);
			}
			else if (line_to_invert<=0){				//Ønsker å bla opp, må scrolle
				line_to_invert=1;
				oled_writeFromStartOnLine(line_to_invert);
				oled_writeString(letters[currentIndex],length,1);
				for(int i = 1;i<number_of_lines-1;i++){
					oled_writeFromStartOnLine(i+line_to_invert);
					oled_writeString(letters[currentIndex+i],length,0);
				}
			}
			else if(line_to_invert>=number_of_lines){	//Ønsker bla ned, må scrolle
				line_to_invert=number_of_lines-1;
				oled_writeFromStartOnLine(line_to_invert);
				oled_writeString(letters[currentIndex],length,1);
				for(int i = 1;i<line_to_invert;i++){
					oled_writeFromStartOnLine(i);
					oled_writeString(letters[currentIndex+i-number_of_lines+1],length,0);
				}
			}
			else{										//Trenger ikke scrolle
				oled_writeFromStartOnLine(line_to_invert);
				oled_writeString(letters[currentIndex],length,1);
			}
			_delay_ms(200);
		}
		
		if(adc_isButtonPressed()){
			if(currentIndex==lettersInAlphabet+3){
				//printf("You entered: %s",currentWord);
				_delay_ms(1);
				sram_setName(CURRENTNAME,currentWord);
				break;
			}
			else if (currentIndex==lettersInAlphabet+2){
				if (asciiOffset==65){
					asciiOffset=97;
				}
				else{
					asciiOffset=65;
				}
				for (int i=0;i<lettersInAlphabet;i++){	//Adding letters as alternatives
					letters[i][0]=(char) asciiOffset+i;
				}
				for(int i=1;i<number_of_lines;i++){
					oled_writeFromStartOnLine(i);
					oled_writeString(letters[currentIndex-line_to_invert+i],length,0);
				}
				oled_writeFromStartOnLine(line_to_invert);
				oled_writeString(letters[lettersInAlphabet+2],length,1);
			}
			else if(currentIndex==lettersInAlphabet+1){
				if(lengthOfCurrentWord){
					currentWord[lengthOfCurrentWord-1]=' ';
					lengthOfCurrentWord-=1;
				}
			}
			else{
				if(lengthOfCurrentWord<length){
					currentWord[lengthOfCurrentWord]=letters[currentIndex][0];
					lengthOfCurrentWord+=1;
				}
			}
			oled_writeFromStartOnLine(0);//Printer overskrift
			oled_writeString(question,questionLength,0);
			oled_writeString(currentWord,14,0);
			_delay_ms(1000);
		}
	}
}


void menu_giveId(menu* _menu,int _id){
	_menu->id=_id;
}

menu* menu_createAllMenu(){
	menu *mainmenu=malloc(sizeof(menu));
	menu *comp=malloc(sizeof(menu));
	menu *tra=malloc(sizeof(menu));
	menu *compStick=malloc(sizeof(menu));
	menu *compSlide=malloc(sizeof(menu));
	menu *compInfo=malloc(sizeof(menu));
	
	menu_makeMenuOnlyName(mainmenu,"Choose mode.");
	menu_makeMenuOnlyName(comp,"Competition.");
	menu_makeMenuOnlyName(tra,"Training.");
	menu_makeMenuOnlyName(compStick,"Joystick control.");
	menu_makeMenuOnlyName(compSlide,"Slider control.");
	menu_makeMenuOnlyName(compInfo,"Info competition mode.");
	
	menu_addChildParent(mainmenu,comp);
	menu_addChildParent(mainmenu,tra);
	menu_addChildParent(comp,compStick);
	menu_addChildParent(comp,compSlide);
	menu_addChildParent(comp,compInfo);
	menu_addParentAsLastChild(comp);
	menu_giveId(compStick,MENU_COMP_STICK);
	menu_giveId(compSlide,MENU_COMP_SLIDE);
	menu_giveId(compInfo,MENU_COMP_INFO);
	menu_giveId(tra,MENU_TRA_SLIDE);
	return mainmenu;
}

void menu_name(){
	menu_enterWord("Enter name:",11);
}


