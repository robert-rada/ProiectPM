/*
 * graphics.c
 *
 * Created: 15/05/2019 7:34:04 PM
 *  Author: Robert
 */ 

#include "graphics.h"
#include "ili9341gfx.h"
#include "player.h"

void drawCircleLine(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y, uint16_t color)
{
	uint16_t nrx = (x > 1 ? 2 * x - 1 : 0);
	uint16_t nry = (y > 1 ? 2 * y - 1 : 0);
	
	ili9341_drawpixel(xc - x, yc + y, BLACK);
	ili9341_drawhline(xc - x + 1, yc + y, nrx, color);
	ili9341_drawpixel(xc + x, yc + y, BLACK);
	
	ili9341_drawpixel(xc - x, yc - y, BLACK);
	ili9341_drawhline(xc - x + 1, yc - y, nrx, color);
	ili9341_drawpixel(xc + x, yc - y, BLACK);
	
	ili9341_drawpixel(xc - y, yc + x, BLACK);
	ili9341_drawhline(xc - y + 1, yc + x, nry, color);
	ili9341_drawpixel(xc + y, yc + x, BLACK);
	
	ili9341_drawpixel(xc - y, yc - x, BLACK);
	ili9341_drawhline(xc - y + 1, yc - x, nry, color);
	ili9341_drawpixel(xc + y, yc - x, BLACK);
}

void drawCircle(uint16_t xc, uint16_t yc, uint8_t r, uint16_t color)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;
	
	drawCircleLine(xc, yc, x, y, color);
	
	while (y >= x)
	{
		x++;
		
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		drawCircleLine(xc, yc, x, y, color);
	}
}

void drawPlayerXXX(uint16_t x, uint16_t y, Player *player)
{
	ili9341_setaddress(x-8, y-8, x+8, y+8);
	
	uint8_t offset[] = {7, 4, 3, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 2, 3, 4, 7};
	uint8_t nr_black[] = {1, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 1};
	uint8_t nr_color[] = {1, 3, 4, 5, 6, 6, 7, 7, 7, 6, 6, 5, 4, 3, 1};

	for (uint8_t i = 0; i <= 16; i++)
	{
		uint16_t color;
		
		for (uint8_t j = 0; j < 8; j++)
		{
			if (j < offset[i])
				color = BLACK;
			else
				color = player->color;
		}
	}
}

void drawNumber(int16_t x, int16_t y, uint16_t number, uint16_t color, uint16_t bg, uint8_t size)
{
	char buffer[8];
	int i = 0;
	
	while (number)
	{
		buffer[i++] = number % 10 + '0';
		number /= 10;
	}
	
	if (i == 0)
		buffer[i++] = '0';
		
	while (i--)
	{
		ili9341_drawchar(x, y, ' ', color, bg, size);
		ili9341_drawchar(x, y, buffer[i], color, bg, size);
		x += size * 6;
	}
}

void drawTimeNumber(int16_t x, int16_t y, uint16_t number, uint16_t color, uint16_t bg, uint8_t size)
{
	x = x - size * 12;
	
	ili9341_drawchar(x, y, ' ', color, bg, size);
	if (number / 600 > 0)
	ili9341_drawchar(x, y, (number / 600) % 10 + '0', color, bg, size);
	x += 6 * size;
	
	ili9341_drawchar(x, y, ' ', color, bg, size);
	ili9341_drawchar(x, y, (number / 60) % 10 + '0', color, bg, size);
	x += 6 * size;
	
	ili9341_drawchar(x, y, ':', color, bg, size);
	x += 6 * size;
	
	ili9341_drawchar(x, y, ' ', color, bg, size);
	ili9341_drawchar(x, y, (number % 60) / 10 + '0', color, bg, size);
	x += 6 * size;
	
	ili9341_drawchar(x, y, ' ', color, bg, size);
	ili9341_drawchar(x, y, number % 10 + '0', color, bg, size);
	x += 6 * size;
}

void drawBackgroundPixel(uint16_t x, uint16_t y)
{
	ili9341_drawpixel(x, y, getBackgroundPixel(x, y));
}

uint16_t getBackgroundPixel(uint16_t x, uint16_t y)
{
	uint16_t dx = abs(SCREEN_WIDTH / 2 - x);
	uint16_t dy = abs(SCREEN_HEIGHT / 2 - y);
	
	const uint16_t line_dist_x = SCREEN_WIDTH * 17 / 40;
	const uint16_t line_dist_y = SCREEN_HEIGHT * 3 / 10;
	const uint16_t line_thickness = 3;
	const uint16_t mid_radius = 40;
	const uint16_t mid_radius_2_min = (mid_radius - line_thickness + 1) * (mid_radius - line_thickness + 1);
	const uint16_t mid_radius_2_max = (mid_radius + line_thickness - 1) * (mid_radius + line_thickness - 1);
	const uint16_t stripe_width = 60;
	const uint16_t goal_x = 22;
	const uint16_t goal_y = 30;
	
	if (dx * dx + dy * dy < mid_radius_2_max && dx * dx + dy * dy > mid_radius_2_min)
		return WHITE;
	if (dx < line_dist_x + line_thickness && dx > line_dist_x - line_thickness
		&& dy < line_dist_y + line_thickness)
		return WHITE;
	if (dy < line_dist_y + line_thickness && dy > line_dist_y - line_thickness
		&& dx < line_dist_x + line_thickness)
		return WHITE;
	if (dx < line_thickness && dy < line_dist_y + line_thickness)
		return WHITE;
	
	if (dx > line_dist_x && dx < line_dist_x + goal_x
		&& ((dy < goal_y + 2 && dy > goal_y - 2)
		|| (dy < goal_y + 2 && dy > goal_y - 2)))
		return WHITE;
	
	if (dx > line_dist_x + goal_x - 2 && dx < line_dist_x + goal_x + 2
		&& dy < goal_y + 2)
		return WHITE;
	
	uint16_t stripe_index = x - (SCREEN_WIDTH / 2 - line_dist_x)
	+ y - (SCREEN_HEIGHT / 2 - line_dist_y);
	
	if (stripe_index % stripe_width < stripe_width / 2
		&& dx < line_dist_x && dy < line_dist_y)
		return DARK_GREEN;
	
	return LIGHT_GREEN;
}

void drawBackground()
{
	uint16_t i,j;
	ili9341_setaddress(0,0,SCREEN_WIDTH-1,SCREEN_HEIGHT-1);

	for(i=0;i<SCREEN_HEIGHT;i++)
	{
		for(j=0;j<SCREEN_WIDTH;j++)
		{
			ili9341_pushcolour(getBackgroundPixel(j, i));
		}
	}
}

void drawHBGLine(uint16_t x,uint16_t y,uint16_t w)
{
	ili9341_setaddress(x, y, x + w - 1, y);
	
	while(w--)
	{
		ili9341_pushcolour(getBackgroundPixel(x, y));
		x++;
	}
}

void drawBackGroundLine(uint16_t xc, uint16_t yc, uint16_t x, uint16_t y)
{
	drawHBGLine(xc - x, yc + y, 2 * x + 1);
	drawHBGLine(xc - x, yc - y, 2 * x + 1);
	drawHBGLine(xc - y, yc + x, 2 * y + 1);
	drawHBGLine(xc - y, yc - x, 2 * y + 1);
}

void drawCircleBackGround(uint16_t xc, uint16_t yc, uint8_t r)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;
	
	drawBackGroundLine(xc, yc, x, y);
	
	while (y >= x)
	{
		x++;
		
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		drawBackGroundLine(xc, yc, x, y);
	}
}

void drawScore(uint8_t score0, uint8_t score1)
{
	drawNumber(SCREEN_WIDTH / 4, 5, score0, BLUE, LIGHT_GREEN, 2);
	
	drawNumber(SCREEN_WIDTH * 3 / 4, 5, score1, RED, LIGHT_GREEN, 2);
}