/*
 * input.c
 *
 * Created: 20/05/2019 9:53:31 PM
 *  Author: Robert
 */ 

#include "input.h"
#include "adc.h"
#include "graphics.h"
#include "ili9341gfx.h"
#include <avr/io.h>
#include <math.h>

#define NR_BUTTONS 4

static volatile uint8_t button_registers[8];
static volatile uint8_t button_pressed[8];

void initInput()
{
	for (int i = 0; i < NR_BUTTONS; i++)
	{
		button_registers[i] = 0xFF;
		button_pressed[i] = 0;
	}
	
	TCCR2A |= (3 << WGM20); // Fast PWM, TOP = 0xFF
	TCCR2B |= (5 << CS20); // clk / 64 ~= 1kHz
	TIMSK2 |= (1 << TOIE2); // enable overflow interrupt
}

void timer2Overflow()
{
	for (int i = 0; i < NR_BUTTONS; i++)
	{
		button_registers[i] <<= 1;
		
		if ((PINC & (1 << i)))
			button_registers[i] |= 1;
			
		if (button_registers[i] == 0)
			button_pressed[i] = 1;
		else if (button_registers[i] == 0xFF)
			button_pressed[i] = 0;
	}
}

uint8_t isPressed(uint8_t button_pin)
{
	return button_pressed[button_pin];
}

void normalize(uint16_t *x, uint16_t *y)
{
	uint16_t nx = *x / 4;
	uint16_t ny = *y / 4;
	
	if (nx > 127)
	nx = 127;
	if (ny > 127)
	ny = 127;
	
	if (nx * nx + ny * ny > 127 * 127)
	{
		uint16_t l = 64;
		uint16_t r = 127;
		
		while (l < r)
		{
			uint16_t mid = (l + r + 1) / 2;
			uint16_t mid_x = nx * mid / 128;
			uint16_t mid_y = ny * mid / 128;
			
			if (mid_x * mid_x + mid_y * mid_y <= 127 * 127)
			l = mid;
			else
			r = mid - 1;
		}
		
		*x = (nx * l / 128) * 4;
		*y = (ny * l / 128) * 4;
	}
}
