#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include "defs.h"
#include <float.h>
#include "player.h"

typedef struct
{
	float rayAngle;
	float wallHitX;
	float wallHitY;
	float distance;

	bool wasHitVertical;
	int wallHitContent;

} ray_t; //NUM_RAYS = WINDOW_WIDTH / WALL_STRIP


extern ray_t rays[NUM_RAYS];


float normalizeAngle(float angle);
float distanceBetweenPoints(float x1, float y1, float x2, float y2);
void castRay(float rayAngle, int stripId);
void castAllRays();
void renderRays();

#endif