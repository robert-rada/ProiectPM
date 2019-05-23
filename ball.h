/*
 * ball.h
 *
 * Created: 23/05/2019 1:32:48 PM
 *  Author: Robert
 */ 


#ifndef BALL_H_
#define BALL_H_

#define BALL_RADIUS 5

#include <inttypes.h>

typedef struct
{
	double x;
	double y;
	double prev_x;
	double prev_y;
	double vx;
	double vy;
	double max_speed;
	double dec;
	double mass;
	
} Ball;

void initBall(Ball *ball);
void updateBall(Ball *ball);
void drawBall(Ball *ball);
void decelerateBall(Ball *ball);
void accelerateBall(Ball *ball, double vx, double vy);
void boundSpeed(Ball *ball);
uint8_t collision(Ball *ball0, Ball *ball1);
void savePrevPos(Ball *ball);
void restorePrevPos(Ball *ball);
void normalizeVector(double *x, double *y);
double dist2(Ball *ball0, Ball *ball1);

#endif /* BALL_H_ */