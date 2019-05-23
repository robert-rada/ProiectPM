/*
 * clock.c
 *
 * Created: 20/05/2019 3:27:59 PM
 *  Author: Robert
 */ 

#include "clock.h"
#include "graphics.h"
#include <avr/io.h>

static volatile uint8_t ticks;
static volatile int16_t time_remaining;
static volatile uint8_t second_counter;
static uint8_t pause;

void initClock(int16_t total_time)
{
	ticks = 0;
	time_remaining = total_time;
	second_counter = 0;
	pause = 1;
	
	TCCR0A |= (3 << WGM00); // Fast PWM, TOP = 0xFF
	TCCR0B |= (5 << CS00); // clk / 1024 ~= 61 Hz
	TIMSK0 |= (1 << TOIE0); // enable overflow interrupt
}

void pauseClock()
{
	pause = 1;
}

void startClock()
{
	ticks = 0;
	pause = 0;
}

void waitClock()
{
	PORTD |= (1 << 7);
	while (ticks < 5);
	PORTD &= ~(1 << 7);
	
	ticks -= 5;
}

void drawTime()
{
	drawTimeNumber(SCREEN_WIDTH / 2, 5, time_remaining, WHITE, LIGHT_GREEN, 2);
}

void timer0Overflow()
{
	if (pause)
		return;
		
	ticks++;
	
	second_counter++;
	if (second_counter == 61)
	{
		time_remaining--;
		second_counter = 0;
	}
}

int16_t getTimeRemaining()
{
	return time_remaining;
}