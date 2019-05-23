/*
 * clock.h
 *
 * Created: 20/05/2019 3:24:23 PM
 *  Author: Robert
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

#include <inttypes.h>

void initClock();
void waitClock();
void drawTime();
void timer0Overflow();
void pauseClock();
void startClock();
int16_t getTimeRemaining();

#endif /* CLOCK_H_ */