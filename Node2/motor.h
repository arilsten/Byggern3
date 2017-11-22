/*
 * motor.h
 *
 * Created: 11.11.2017 17:23:18
 *  Author: Arild Stenset
 */ 

 #include <stdint.h>
 #include "can.h"

#ifndef MOTOR_H_
#define MOTOR_H_

void motor_init();
void motor_speed(uint8_t speed);
void motor_dir(int direction);
void motor_driver(can_message_t message);
void motor_encoderInit();
uint16_t motor_encoderRead(void);
void motor_encoderReset();
void motor_encoderValuePrint();
void motor_test();

int motor_PIcontroller(int position, float Kp, float Ki, float sampleTime, int reference);
void motor_speedReg(can_message_t message);

void motor_getEncoderLimits();
void printEncoderValues();

#endif /* MOTOR_H_ */