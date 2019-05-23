/*
 * adc.c
 *
 * Created: 15/05/2019 8:36:32 PM
 *  Author: Robert
 */ 

#include "avr/io.h"
#include "adc.h"

void adcInit()
{
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (5 << ADPS0);
}

uint16_t adcGet(uint8_t channel)
{
	ADMUX = (ADMUX & ~(0x1f << MUX0)) | channel;

	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));

	return ADC;
}