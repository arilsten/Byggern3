/*
 * timer.h
 *
 * Created: 18.11.2017 15:25:54
 *  Author: Arild Stenset
 */ 


#ifndef TIMER_H_
#define TIMER_H_

void timer_start(void);
void timer_stop(void);
uint16_t timer_get();
void timer_reset(void);


#endif /* TIMER_H_ */