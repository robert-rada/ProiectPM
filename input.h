/*
 * input.h
 *
 * Created: 20/05/2019 9:53:23 PM
 *  Author: Robert
 */ 


#ifndef INPUT_H_
#define INPUT_H_

#include <inttypes.h>
#include "vector2.h"

void initInput();
uint8_t isPressed(uint8_t button_pin);
void timer2Overflow();
void normalize(uint16_t *x, uint16_t *y);

#endif /* INPUT_H_ */