/*
 * graphics.h
 *
 * Created: 15/05/2019 7:32:59 PM
 *  Author: Robert
 */ 


#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "ili9341.h"
#include<inttypes.h>

#define DARK_GREEN 0b0000001111100101
#define LIGHT_GREEN 0b0100110100000101
#define WHITE 0b1110011100111100

#define BORDER_DIST_X (SCREEN_WIDTH * 17 / 40)
#define BORDER_DIST_Y (SCREEN_HEIGHT * 3 / 10)
#define GOAL_X 22
#define GOAL_Y 28

void drawCircle(uint16_t x, uint16_t y, uint8_t radius, uint16_t color);
void drawCircleBackGround(uint16_t x, uint16_t y, uint8_t radius);
void drawNumber(int16_t x, int16_t y, uint16_t number, uint16_t color, uint16_t bg, uint8_t size);
void drawTimeNumber(int16_t x, int16_t y, uint16_t number, uint16_t color, uint16_t bg, uint8_t size);
void drawBackgroundPixel(uint16_t x, uint16_t y);
uint16_t getBackgroundPixel(uint16_t x, uint16_t y);
void drawBackground();
void drawScore(uint8_t score0, uint8_t score1);

#endif /* GRAPHICS_H_ */