/*
 * motor.c
 *
 * Created: 11.11.2017 17:23:04
 *  Author: Arild Stenset
 */ 
 #define F_CPU 16000000UL
 #include <util/delay.h>
 #include <stdint.h>
 #include <avr/io.h>
 #include <stdio.h>

 #include "motor.h"
 #include "max520.h"
 #include "can.h"
 #include "timer.h"

  #define MOTOR_DIR_LEFT		0x00
  #define MOTOR_DIR_RIGHT		0x01
  #define MOTOR_STOP			0x00

 void motor_init(){
	DDRH = (1 << PH4);			// Motor enable pin as output
	DDRH |= (1 << PH1);			// Motor direction pin as output
	PORTH |= (1 << PH4);		// Enable Motor
 }

 void motor_speed(uint8_t speed){
	max520_send(speed);

 }

 void motor_dir(int direction){
	if(direction){
		PORTH |= (1 << PH1);
	} 
	else{
		PORTH &= ~(1 << PH1);
	}
 }

 void motor_driver(can_message_t message){
		if(message.data[0] > 135){
			motor_dir(MOTOR_DIR_RIGHT);
			motor_speed((2*message.data[0]) - 255);		//Scaling from 0-255
		}
		else if(message.data[0] < 120){
			motor_dir(MOTOR_DIR_LEFT);
			motor_speed(255 - (2*message.data[0]));		//Scaling from 0-255
		}
		else{
			motor_speed(0);
		}
 }

 void motor_encoderInit(){							// DIR - PH1, SEL - PH3, !OE - PH5, EN - PH4, RST - PH6
	DDRH |= (1 << PH5) | (1 << PH3) | (1 << PH6);	// Set !OE, SEL, RST as output
	PORTH |= (1 << PH5) | (1 << PH6);				// Set !OE and RST to High
	DDRK = 0x00;									// PORT K as input
 }

 uint16_t motor_encoderRead(void){
   PORTH &= ~(1 << PH5);			// Set !OE low to enable output of encoder
   PORTH &= ~(1 << PH3);			// Lower SEL to get high byte
   _delay_ms(20);					// Wait about 20 microseconds
   uint8_t msb = PINK;				// Read MSB
   PORTH |= (1 << PH3);				// Set SEL high to get low byte
   _delay_ms(20);					// Wait about 20 microseconds
   uint8_t lsb = PINK;				// Read LSB
   //PORTH &= ~(1 << PH6);			// Toggle !RST to reset encoder
   //PORTH |= (1 << PH6);				// Set !RST high
   //motor_encoderReset();
   PORTH |= (1 << PH5);				// Set !OE high to disable output of encoder
   return ((msb << 8) | lsb);		// Process received data....
}

 void motor_encoderReset(){
	PORTH &= ~(1 << PH6);
	PORTH |= (1 << PH6);
 }
 
 void motor_encoderValuePrint(){
	printf("EncoderValue: %d\n", motor_encoderRead());
	_delay_ms(100);
 }

 void motor_test(){
	 motor_dir(MOTOR_DIR_LEFT);
	 motor_speed(0x50);
	 _delay_ms(300);
	 motor_dir(MOTOR_DIR_RIGHT);
	 motor_speed(0x50);
	 _delay_ms(300);
 }

  int lastPosition = 0;									//Values from 0 to ffff
  int errorSum = 0;

  int motor_PIcontroller(int position, float Kp, float Ki, float sampleTime, int reference){
	  
	  int currentSpeed = position - lastPosition;
	  
	  if (currentSpeed>0x6000){							// If we go from 0 to ffff
		  currentSpeed=-0x8000;
	  }
	  else if(currentSpeed<-0x6000){					// if we go from ffff to 0
		  currentSpeed+=0x8000;
	  }
	  currentSpeed*=0.01275/sampleTime;					// Values from -255 to 255
														// printf("speed: %d \t speedREf: %d \t",currentSpeed,reference);
	  int error = reference - currentSpeed;
	  
	  errorSum += error;
	  lastPosition = position;
	  
	  return (int) (currentSpeed+(Kp*error + sampleTime*Ki*errorSum));
  }

  void motor_speedReg(can_message_t message){
	  int speedRef = 2*message.data[0] - 255;
	  int Kp = 1;
	  float Ki = 0.5;
	  float sampleTime = 0.00014;
	  int position = motor_encoderRead();
	  int regulatedSpeed = motor_PIcontroller(position, Kp, Ki, sampleTime, speedRef);

	  if(regulatedSpeed>255){						// passer på at vi har gyldige verdier
		  regulatedSpeed=255;
	  }
	  else if (regulatedSpeed<-255){
		  regulatedSpeed=-255;
	  }
	  
	  if (regulatedSpeed>0){
		  motor_dir(MOTOR_DIR_RIGHT);
		  motor_speed(regulatedSpeed);
	  }
	  else{
		  motor_dir(MOTOR_DIR_LEFT);
		  motor_speed(-regulatedSpeed);
	  }
  }


  int eSumPos=0;
  int encoderLeft = 0;
  int encoderRight = 0;

  void motor_getEncoderLimits(){
	motor_dir(MOTOR_DIR_RIGHT);
	motor_speed(100);
	_delay_ms(2000);
	motor_speed(200);
	int temp=motor_encoderRead();
	while(!(-1000<temp && 1000>temp)){
		temp=motor_encoderRead();
	}
	motor_speed(0);
	_delay_ms(20);
	encoderRight = motor_encoderRead();
	printf("EncoderLimitRight: %d\n", encoderRight);
	motor_dir(MOTOR_DIR_LEFT);
	motor_speed(100);
	_delay_ms(2000);
	encoderLeft = motor_encoderRead();
	printf("EncoderLimitLeft: %d\n", encoderLeft);
	motor_speed(0);
	timer_reset();
  }

  void printEncoderLimits(){
	printf("EncoderLimitLeft: %d\n", encoderLeft);
	printf("EncoderLimitRight: %d\n", encoderRight);
  }

 int lastTime = 0;
 int currentTime = 0;
 int sumSampleTime=0;
 int counterSampleTime=0;

void motor_posReg(can_message_t message){
	int enc = motor_encoderRead();

	if (enc < encoderRight){							// To better handle drift in the encoder value
		int dif = encoderRight - enc;
		encoderRight = enc;
		encoderLeft = encoderLeft - dif;
	}
	else if (encoderLeft < enc){
		int dif = enc - encoderLeft;
		encoderLeft = enc;
		encoderRight = encoderRight + dif;
	}

	int pos = ((enc - encoderRight)*0.0283);			// b ish 0.0283 = 255/9000
	int ref = (255 - message.data[0]);					// inverse

	float Kp = 0.7;
	float Ki = 0.02;

	int sampletime = timer_get();
	timer_reset();
	timer_start();

	int ePos = ref-pos;
	eSumPos = ePos+eSumPos;
	int speed = Kp*ePos + Ki*eSumPos*sampletime;

	  if (speed<-255){
		  speed=-255;
	  }
	  else if (255<speed){
		  speed=255;
	  }
	  else if (-6<speed && speed<6){
		  speed=0;
	  }

	if(speed>0){
		motor_dir(MOTOR_DIR_LEFT);
		motor_speed(speed);		
	}
	else{
		motor_dir(MOTOR_DIR_RIGHT);
		motor_speed(-speed);
	}
}