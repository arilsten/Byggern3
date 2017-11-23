/*
 * menu.h
 *
 * Created: 04.10.2017 16:39:22
 *  Author: eddas
 */ 

#include <stdlib.h>
#include <avr/io.h>

#ifndef MENU_H_
#define MENU_H_

//----------------------------------Menu ID----------------------------------------- Must be under 100
#define MENU_INVALIDE	0
#define MENU_ENTERNAME	1
#define MENU_2			2
#define MENU_3			3
#define MENU_COMP_SLIDE 4	//Competition mode with slider control
#define MENU_COMP_STICK 5	//Competition mode with Joystick control
#define MENU_TRA_SLIDE	6
#define MENU_TRA_STICK	7	//Training mode with Joystick control

//--------------------------------InfoId--------------------------------------------- All over 100
#define MENU_COMP_INFO	101
#define MENU_TRA_INFO	102

typedef struct menu menu;
struct menu{
	menu *parent;
	menu *children[10];
	int numb_children;
	char name[20];
	int id;
};

int menu_funct();
void menu_makeMenu(menu* newmenu, menu *_parent,menu **_children,int _numb_children,char* _name);
void menu_makeMenuOnlyName(menu* newmenu,char* _name);
void menu_addChildParent(menu* _parent, menu* _child);
void menu_addParentAsLastChild(menu* _menu);
void menu_printInfo(int id);
int menu_control(menu* current_menu);
int menu_step(menu* _menu);
void menu_addSpaces(char* word,int startIndex,int size);
void menu_enterWord(char* question, int questionLength);
void menu_giveId(menu* _menu,int _id);
menu* menu_createAllMenu();
void menu_name();





#endif /* MENU_H_ */