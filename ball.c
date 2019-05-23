/*
 * ball.c
 *
 * Created: 23/05/2019 1:33:02 PM
 *  Author: Robert
 */ 

#include "ball.h"
#include "graphics.h"

void initBall(Ball *ball)
{
	ball->x = SCREEN_WIDTH / 2;
	ball->y = SCREEN_HEIGHT / 2;
	
	ball->vx = 0;
	ball->vy = 0;	
	
	ball->max_speed = 10;
	ball->dec = 0.05;
	ball->mass = 1;
}

void updateBall(Ball *ball)
{
	decelerateBall(ball);
	boundSpeed(ball);
	
	double new_x = ball->x + ball->vx;
	double new_y = ball->y + ball->vy;
	
	// check horizontal field bounds
	double x0 = SCREEN_WIDTH / 2 - BORDER_DIST_X;
	double x1 = SCREEN_WIDTH / 2 + BORDER_DIST_X;
	double y0 = SCREEN_HEIGHT / 2 - BORDER_DIST_Y;
	double y1 = SCREEN_HEIGHT / 2 + BORDER_DIST_Y;
	
	if ((new_y + BALL_RADIUS > y1 || new_y - BALL_RADIUS < y0) && new_x > x0 && new_x < x1)
		ball->vy *= -1;
		
	new_x = ball->x + ball->vx;
	new_y = ball->y + ball->vy;
		
	// check horizontal gate left
	x0 = SCREEN_WIDTH / 2 - BORDER_DIST_X - GOAL_X;
	x1 = SCREEN_WIDTH / 2 - BORDER_DIST_X;
	y0 = SCREEN_HEIGHT / 2 - GOAL_Y;
	y1 = SCREEN_HEIGHT / 2 + GOAL_Y;
		
	if ((new_y + BALL_RADIUS > y1 || new_y - BALL_RADIUS < y0) && new_x > x0 && new_x < x1)
		ball->vy *= -1;
	
	new_x = ball->x + ball->vx;
	new_y = ball->y + ball->vy;
	
	// check horizontal gate right
	x0 = SCREEN_WIDTH / 2 + BORDER_DIST_X;
	x1 = SCREEN_WIDTH / 2 + BORDER_DIST_X + GOAL_X;
	y0 = SCREEN_HEIGHT / 2 - GOAL_Y;
	y1 = SCREEN_HEIGHT / 2 + GOAL_Y;
	
	if ((new_y + BALL_RADIUS > y1 || new_y - BALL_RADIUS < y0) && new_x > x0)
		ball->vy *= -1;
	
	new_x = ball->x + ball->vx;
	new_y = ball->y + ball->vy;
	
	// check vertical field up
	x0 = SCREEN_WIDTH / 2 - BORDER_DIST_X;
	x1 = SCREEN_WIDTH / 2 + BORDER_DIST_X;
	y0 = SCREEN_HEIGHT / 2 - BORDER_DIST_Y;
	y1 = SCREEN_HEIGHT / 2 - GOAL_Y;
	
	if ((new_x + BALL_RADIUS > x1 || new_x - BALL_RADIUS < x0) && new_y > y0 && new_y < y1)
		ball->vx *= -1;
	
	new_x = ball->x + ball->vx;
	new_y = ball->y + ball->vy;
	
	// check vertical field down
	x0 = SCREEN_WIDTH / 2 - BORDER_DIST_X;
	x1 = SCREEN_WIDTH / 2 + BORDER_DIST_X;
	y0 = SCREEN_HEIGHT / 2 + GOAL_Y;
	y1 = SCREEN_HEIGHT / 2 + BORDER_DIST_Y;
	
	if ((new_x + BALL_RADIUS > x1 || new_x - BALL_RADIUS < x0) && new_y > y0 && new_y < y1)
	ball->vx *= -1;
	
	new_x = ball->x + ball->vx;
	new_y = ball->y + ball->vy;
	
	// check vertical gate
	x0 = SCREEN_WIDTH / 2 - BORDER_DIST_X - GOAL_X;
	x1 = SCREEN_WIDTH / 2 + BORDER_DIST_X + GOAL_X;
	y0 = SCREEN_HEIGHT / 2 - GOAL_Y;
	y1 = SCREEN_HEIGHT / 2 + GOAL_Y;
	
	if ((new_x + BALL_RADIUS > x1 || new_x - BALL_RADIUS < x0) && new_y > y0 && new_y < y1)
		ball->vx *= -1;
	
	new_x = ball->x + ball->vx;
	new_y = ball->y + ball->vy;
	
	ball->x += ball->vx;
	ball->y += ball->vy;
}

void decelerateBall(Ball *ball)
{
	if (fabs(ball->vx) > 0.001 && fabs(ball->vy) > 0.001)
	{
		double mag = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);
		double vx = ball->vx / mag * ball->dec;
		double vy = ball->vy / mag * ball->dec;
		
		if (fabs(ball->vx) < fabs(vx))
			ball->vx = 0;
		else
			ball->vx -= vx;
			
		if (fabs(ball->vy) < fabs(vy))
			ball->vy = 0;
		else
			ball->vy -= vy;
	}
}

void drawBall(Ball *ball)
{
	drawCircleBackGround(ball->prev_x, ball->prev_y, BALL_RADIUS);
	
	drawCircle(ball->x, ball->y, BALL_RADIUS, WHITE);
}

void boundSpeed(Ball *ball)
{
	double mag = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);
	
	if (mag > ball->max_speed)
	{
		ball->vx = ball->vx / mag * ball->max_speed;
		ball->vy = ball->vy / mag * ball->max_speed;
	}
}

void accelerateBall(Ball *ball, double vx, double vy)
{
	ball->vx += vx;
	ball->vy += vy;
	boundSpeed(ball);
}

void normalizeVector(double *x, double *y)
{
	double mag = sqrtf((*x) * (*x) + (*y) * (*y));
	*x /= mag;
	*y /= mag;
}

// returns 0 if the balls do not collide
uint8_t collision(Ball *ball0, Ball *ball1)
{
	double dx = ball0->x - ball1->x;
	double dy = ball0->y - ball1->y;
	
	if (dx * dx + dy * dy > 14 * 14)
		return 0;
	
	double vx = ball0->vx + ball1->vx;
	double vy = ball0->vy + ball1->vy;
	
	double axis1_x = ball1->x - ball0->x;
	double axis1_y = ball1->y - ball0->y;
	
	double axis2_x = -axis1_y;
	double axis2_y = axis1_x;
	
	normalizeVector(&axis1_x, &axis1_y);
	normalizeVector(&axis2_x, &axis2_y);
	
	double v1x_x = (ball0->vx * axis2_x + ball0->vy * axis2_y) * axis2_x;
	double v1x_y = (ball0->vx * axis2_x + ball0->vy * axis2_y) * axis2_y;
	
	double v1y_x = (ball0->vx * axis1_x + ball0->vy * axis1_y) * axis1_x;
	double v1y_y = (ball0->vx * axis1_x + ball0->vy * axis1_y) * axis1_y;
	
	double v2x_x = (ball1->vx * axis2_x + ball1->vy * axis2_y) * axis2_x;
	double v2x_y = (ball1->vx * axis2_x + ball1->vy * axis2_y) * axis2_y;
	
	double v2y_x = (ball1->vx * axis1_x + ball1->vy * axis1_y) * axis1_x;
	double v2y_y = (ball1->vx * axis1_x + ball1->vy * axis1_y) * axis1_y;
	
	ball0->vx = v1x_x + v2y_x;
	ball0->vy = v1x_y + v2y_y;
	
	ball1->vx = v2x_x + v1y_x;
	ball1->vy = v2x_y + v1y_y;
	
	return 1;
}

void savePrevPos(Ball *ball)
{
	ball->prev_x = ball->x;
	ball->prev_y = ball->y;
}

void restorePrevPos(Ball *ball)
{
	ball->x = ball->prev_x;
	ball->y = ball->prev_y;
}

double dist2(Ball *ball0, Ball *ball1)
{
	double dx = ball0->x - ball1->x;
	double dy = ball0->y - ball1->y;
	
	return dx * dx + dy * dy;
}