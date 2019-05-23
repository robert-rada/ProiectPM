/*
 * adc.h
 *
 * Created: 15/05/2019 8:36:18 PM
 *  Author: Robert
 */ 


#ifndef ADC_H_
#define ADC_H_

#include<inttypes.h>

uint16_t adcGet(uint8_t channel);
void adcInit();

#endif /* ADC_H_ */