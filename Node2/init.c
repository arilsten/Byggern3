/*
 * init.c
 *
 * Created: 08.11.2017 10:25:11
 *  Author: Arild Stenset
 */ 

 #define F_CPU 16000000UL
 #define BAUD 9600
 #define MYUBRR (F_CPU/16/BAUD - 1)		//If this doesn't work, set MYUBRR to 103. Check Table 22-12.

 #include <avr/interrupt.h>
 #include <avr/io.h>

 #include "init.h"
 #include "uart.h"
 #include "spi.h"
 #include "MCP2515.h"
 #include "can.h"
 #include "pwm.h"
 #include "adc.h"
 #include "TWI_Master.h"
 #include "max520.h"
 #include "motor.h"
 #include "game.h"
 #include "solenoid.h"
 #include "timer.h"
 
 void initialize(){
	uart_init(MYUBRR);
	spi_masterInit();
	mcp2515_init();
	can_init();
	mcp2515_bitModify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);			//Set mcp in normal mode
	pwm_init();
	adc_init();
	TWI_Master_Initialise();
	max520_init();
	motor_init();
	motor_encoderInit();
	game_timerInit();
	solenoid_init();
	timer_start();
	motor_getEncoderLimits();										// Runs the motor back and forth and notes maximum and minimum values of encoder.
	
 }