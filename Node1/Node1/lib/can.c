/*
 * can.c
 *
 * Created: 18.10.2017 14:21:52
 *  Author: eddas, arillas, robinas
 */ 

#include <stdio.h>

#include "can.h"
#include "MCP2515.h"

volatile int can_message_received = 0;

void can_setMessageReceived(){
	can_message_received = 1;
}

void can_clearMessageReceived(){
	mcp2515_bitModify(MCP_CANINTF, MCP_RX0IF, 0x00);
	can_message_received = 0;
}

int can_getMessageReceived(){
	return can_message_received;
}

void can_init(){
	mcp2515_bitModify(MCP_RXB0CTRL, MCP_FILTER_MASK, 0xFF);		// Filter Off
	mcp2515_bitModify(MCP_CANINTE, MCP_RX0IE, MCP_RX0IE);		// Should generate interrupt on INT pin of mcp upon receivement of a message.
	MCUCR |= (1 << ISC01);										//interrupt on falling edge for mcp/can.
	MCUCR &= ~(1 << ISC00);										//interrupt on falling edge for mcp/can.
	GICR |= (1 << INT0);										//Enable interrupt on INT0
	mcp2515_bitModify(MCP_BFPCTRL, MCP_B0BFE, MCP_B0BFE);		//
	mcp2515_bitModify(MCP_BFPCTRL, MCP_B0BFM, MCP_B0BFM);
	can_read();
}

void can_sendMessage(can_message_t message){
	while(mcp2515_read(MCP_TXB0CTRL) & (1 << MCP_TXREQ)){}		//
	mcp2515_bitModify(MCP_CANINTE, 0xFF, MCP_TX0IE);
	mcp2515_write(MCP_TXB0SIDH, (message.id >> 3));				// Bitshifts and puts an 11-bit identity into ID low and high register.
	mcp2515_write(MCP_TXB0SIDL, (uint8_t)(message.id << 5));	// -----------------------||-------------------------	
	mcp2515_write(MCP_TXB0DLC, message.length);					// Set the 4-bit datalength in the DLC register.
	for (int i=0;i<message.length;i++){
		mcp2515_write(MCP_TXB0D0 + i, message.data[i]);
	}	
	mcp2515_RTS();
}

can_message_t can_read(){
	can_message_t message;
	
	message.id = ((mcp2515_read(MCP_RXB0SIDH) << 3) | (mcp2515_read(MCP_RXB0SIDL) >> 5));
	message.length = mcp2515_read(MCP_RXB0DLC);
		
	for (int i = 0; i < message.length; i++){
		message.data[i] = mcp2515_read(MCP_RXB0D0 + i);
	}
	
	mcp2515_bitModify(MCP_CANINTF, MCP_RX0IF, 0x00);			//Second argument should be RX0IF, NOT TX0IF!
	can_clearMessageReceived();
	return message;
}

void can_print(can_message_t message){
	printf("CAN-bus message: (id:%x, len:%d, data:{", message.id, message.length);
	if(message.length){
		printf("%d", message.data[0]);
	}
	for(uint8_t i = 1; i < message.length; i++){
		printf(", %d", message.data[i]);
	}
	printf("})\n");
}