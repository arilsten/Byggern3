/*
 * timer.c
 *
 * Created: 18.11.2017 15:25:36
 *  Author: Arild Stenset
 */ 

 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <stdio.h>

 uint8_t volatile timer = 0;

 void timer_start(){
	 TCCR5B |=  (1<<CS51);				// Prescaler set to 8
	 TIMSK5 |= (1 << TOIE5);            // Overflow interrupt
 }

 void timer_stop(){
	 TCCR5B &= ~(1 << CS50);
	 TCCR5B &= ~(1 << CS52);
	 TIMSK5 &= ~(1 << TOIE5);
 }

 uint16_t timer_get(){
	 return timer;
 }

 void timer_reset(){
	 timer = 0;
 }

ISR(TIMER5_OVF_vect){
	timer++;
	//printf("Timer: %d\n", timer);
}