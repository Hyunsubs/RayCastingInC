#ifndef MAP_H
#define MAP_H
#include "defs.h"
#include <stdbool.h>

bool mapHasWallAt(float x, float y);
void renderMap();
int getMapAt(int i, int j);

#endif