#ifndef MAP_H
#define MAP_H
#include "defs.h"
#include "graphics.h"
#include <stdbool.h>

#define MAP_NUM_ROWS 13 
#define MAP_NUM_COLS 20

bool mapHasWallAt(float x, float y);
void renderMap();
int getMapAt(int i, int j);
bool isInsideMap(float x,float y);

#endif