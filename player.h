/*
 * player.h
 *
 * Created: 20/05/2019 8:17:25 PM
 *  Author: Robert
 */ 


#ifndef PLAYER_H_
#define PLAYER_H_

#include "vector2.h"
#include "ball.h"
#include <inttypes.h>

#define PLAYER_SPEED 1
#define PLAYER_RADIUS 8
#define KICK_SPEED 15

typedef struct  
{
	Ball ball;
	uint8_t vx_channel;
	uint8_t vy_channel;
	uint8_t button_pin;
	uint16_t color;
	uint8_t id;
	int8_t kick; // 0 = kick ready, 1 = kicking, -1 = button still pressed after kick
} Player;

void initPlayer(Player *player, uint8_t id);
void update(Player *player);
void drawPlayer(Player *player);
void decelerate(Player *player);
void updateKick(Player *player);
void kickBall(Player *player);

#endif /* PLAYER_H_ */