/*
 * Haxball.c
 *
 * Created: 15/05/2019 5:34:57 PM
 * Author : Robert
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ili9341.h"
#include "graphics.h"
#include "ili9341gfx.h"
#include "adc.h"
#include "clock.h"
#include "input.h"
#include "player.h"
#include "ball.h"
#include <string.h>

void init()
{
	// init buzzer
	DDRD |= (1 << 4);
	PORTD |= (1 << 4);
	
	// init LED
	DDRD |= (1 << 7);
	
	// init PB2
	DDRB  &= ~(1 << 2);
	PORTB |= (1 << 2);
	
	// init PC0
	DDRC  &= ~(1 << 0);
	PORTC |= (1 << 0);
	
	// init PC1
	DDRC  &= ~(1 << 1);
	PORTC |= (1 << 1);
	
	// init PC2
	DDRC  &= ~(1 << 2);
	PORTC |= (1 << 2);
	
	// init PC3
	DDRC  &= ~(1 << 3);
	PORTC |= (1 << 3);
}

ISR(TIMER0_OVF_vect)
{
	timer0Overflow();
}

ISR(TIMER2_OVF_vect)
{
	timer2Overflow();
}

Ball ball;
Player player0, player1;

void resetGame()
{
	drawBackground();
	initPlayer(&player0, 0);
	initPlayer(&player1, 1);
	initBall(&ball);
}

void startScreen()
{
	ili9341_clear(BLACK);
	
	const char text[] = "Press both\nbuttons to\nstart the game";
	uint16_t x = SCREEN_WIDTH / 8;
	uint16_t y = SCREEN_HEIGHT / 3;
	uint8_t text_size = 3;
	
	for (int i = 0; i < sizeof(text); i++)
	{
		if (text[i] == '\n')
		{
			y += 10 * text_size;
			x = SCREEN_WIDTH / 8;
		}
		else
		{
			ili9341_drawchar(x, y, text[i], WHITE, BLACK, text_size);
			x += text_size * 6;
		}
	}
		
	while (!isPressed(2) || !isPressed(3));
}

void endScreen(uint8_t score0, uint8_t score1)
{
	char blue_win[] = "Blue wins!";
	char red_win[] = "Red wins!";
	char tie_text[] = "Tie!";
	
	pauseClock();
	
	drawBackground();
	
	drawScore(score0, score1);
	
	uint16_t color = DARKGREY;
	char *text = tie_text;
	
	if (score0 > score1)
	{
		color = BLUE;
		text = blue_win;
	}
	else if (score1 > score0)
	{
		color = RED;
		text = red_win;
	}

	uint8_t text_size = 2;
	uint16_t x = SCREEN_WIDTH / 2 - text_size * 3 * strlen(text);
	uint16_t y = 20;

	for (int i = 0; text[i] != 0; i++)
	{
		ili9341_drawchar(x, y, text[i], color, LIGHT_GREEN, text_size);
		x += text_size * 6;
	}
	
	while (!isPressed(2) || !isPressed(3));
}

void startGame()
{
	startScreen();
	
	uint8_t player0_score = 0, player1_score = 0;
	
	initClock(120);
	
	resetGame();
	
	startClock();
	
	while (getTimeRemaining() > 0)
	{
		waitClock();
		
		drawTime();
		drawScore(player0_score, player1_score);
		
		savePrevPos(&ball);
		savePrevPos(&(player0.ball));
		savePrevPos(&(player1.ball));
		
		update(&player0);
		update(&player1);
		
		updateBall(&ball);
		
		uint8_t coll = 1;
		uint8_t counter = 10;
		while (coll)
		{
			coll = 0;
			
			if (counter-- == 0)
			{
				break;
			}
			
			if (collision(&ball, &(player1.ball)))
			{
				restorePrevPos(&ball);
				restorePrevPos(&(player1.ball));
				updateBall(&ball);
				updateBall(&(player1.ball));
				coll = 1;
			}
			
			if (collision(&ball, &(player0.ball)))
			{
				restorePrevPos(&ball);
				restorePrevPos(&(player0.ball));
				updateBall(&ball);
				updateBall(&(player0.ball));
				coll = 1;
			}
			
			if (collision(&(player1.ball), &(player0.ball)))
			{
				restorePrevPos(&(player1.ball));
				restorePrevPos(&(player0.ball));
				updateBall(&(player1.ball));
				updateBall(&(player0.ball));
				coll = 1;
			}
		}
		
		if (counter == 0)
		{
			resetGame();
			continue;
		}
		
		if (ball.x < SCREEN_WIDTH / 2 - BORDER_DIST_X - 5)
		{
			player1_score++;
			pauseClock();
			resetGame();
			startClock();
			continue;
		}
		
		if (ball.x > SCREEN_WIDTH / 2 + BORDER_DIST_X + 5)
		{
			player0_score++;
			pauseClock();
			resetGame();
			startClock();
			continue;
		}
		
		drawPlayer(&player0);
		drawPlayer(&player1);
		drawBall(&ball);
	}
	
	endScreen(player0_score, player1_score);
}

int main(void)
{
	init();
	
	ili9341_init();
	ili9341_setRotation(1);
	
	adcInit();
	initInput();
	
	sei();
	
	while (1)
	{
		startGame();
	}
}

