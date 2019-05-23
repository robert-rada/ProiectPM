/*
 * player.c
 *
 * Created: 20/05/2019 8:17:34 PM
 *  Author: Robert
 */ 

#include "player.h"
#include "graphics.h"
#include "input.h"
#include "adc.h"

void initPlayer(Player *player, uint8_t id)
{
	initBall(&player->ball);
	
	player->ball.y = SCREEN_HEIGHT / 2;
	player->id = id;
	
	if (id == 0)
	{
		player->ball.x = SCREEN_WIDTH / 4;
		player->color = BLUE;
		player->vx_channel = 3;
		player->vy_channel = 2;
		player->button_pin = 2;
	}
	else if (id == 1)
	{
		player->ball.x = SCREEN_WIDTH * 3 / 4;
		player->color = RED;
		player->vx_channel = 1;
		player->vy_channel = 0;
		player->button_pin = 3;
	}
	
	player->ball.prev_x = player->ball.x;
	player->ball.prev_y = player->ball.y;
	
	player->ball.vx = 0;
	player->ball.vy = 0;
	player->ball.max_speed = 5;
	player->ball.dec = 0.3;
	player->ball.mass = 5;
	
	player->kick = 0;
}

void updateKick(Player *player)
{
	if (player->kick == -1)
	{
		if (!isPressed(player->button_pin))
			player->kick = 0;
	}
	else if (player->kick == 0)
	{
		if (isPressed(player->button_pin))
			player->kick = 1;
	}
	else if (player->kick == 1)
	{
		if (!isPressed(player->button_pin))
			player->kick = 0;
	}
}

extern Ball ball;

void kickBall(Player *player)
{
	double vx = ball.x - player->ball.x;
	double vy = ball.y - player->ball.y;
	
	normalizeVector(&vx, &vy);
	
	vx *= KICK_SPEED;
	vy *= KICK_SPEED;
	
	accelerateBall(&ball, vx, vy);
}

void update(Player *player)
{
	updateKick(player);
	
	if (player->kick == 1 && dist2(&ball, &player->ball) < 400)
	{
		kickBall(player);
		player->kick = -1;
	}
	
	player->ball.prev_x = player->ball.x;
	player->ball.prev_y = player->ball.y;
	
	uint16_t dy = adcGet(player->vx_channel);
	uint16_t dx = 1023 - adcGet(player->vy_channel);
	
	uint8_t negative_x = (dx >= 512 ? 0 : 1);
	uint8_t negative_y = (dy >= 512 ? 0 : 1);
	
	dx = (dx >= 512 ? dx - 512 : 512 - dx);
	dy = (dy >= 512 ? dy - 512 : 512 - dy);
	
	normalize(&dx, &dy);
	
	double vx, vy;
	
	if (negative_x)
		vx = -(double)dx / 300;
	else
		vx = (double)dx / 300;
	
	if (negative_y)
		vy = -(double)dy / 300;
	else
		vy = (double)dy / 300;
	
	accelerateBall(&player->ball, vx, vy);
	
	updateBall(&player->ball);
}

void drawPlayer(Player *player)
{
	drawCircleBackGround(player->ball.prev_x, player->ball.prev_y, PLAYER_RADIUS);
	
	if (player->kick == 1)
		drawCircle(player->ball.x, player->ball.y, PLAYER_RADIUS, YELLOW);
	else	
		drawCircle(player->ball.x, player->ball.y, PLAYER_RADIUS, player->color);
}
