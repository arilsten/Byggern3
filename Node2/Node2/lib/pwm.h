/*
 * pwmDriver.h
 *
 * Created: 01.11.2017 16:01:04
 *  Author: Arild Stenset
 */ 

 #include <avr/io.h>
 #include "can.h"

#ifndef PWMDRIVER_H_
#define PWMDRIVER_H_

#define PWM_MIN	15
#define PWM_MAX	31
#define PWM_MID	((PWM_MIN + PWM_MAX)/2)

void pwm_init(void);
void pwm_setValue(uint8_t value);
void pwm_test();
void pwm_setServo(can_message_t msg);

#endif /* PWMDRIVER_H_ */