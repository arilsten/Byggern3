/*
 * can.h
 *
 * Created: 18.10.2017 14:22:04
 *  Author: eddas
 */ 

 #include <stdint.h>

#ifndef CAN_H_
#define CAN_H_

#define SERVO_ID		0x0013
#define MOTOR_ID		0x0023
#define SOLENOID_ID		0x0033
#define GAME_MESSAGE_ID	0x0043

typedef struct can_message_t can_message_t;
struct can_message_t{
	uint16_t id;
	uint8_t length;
	uint8_t data[8];
};

void can_init();
void can_sendMessage(can_message_t message);
can_message_t can_read();
void can_print(can_message_t message);

#endif /* CAN_H_ */

