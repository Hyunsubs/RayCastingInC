#include "player.h"


player_t player = {
	//C99 문법임
	//.무언가로 멤버들을 표시
	// TODO:
	// initialize and setup game objects
	.x = WINDOW_WIDTH / 2, //center
	.y = WINDOW_HEIGHT / 2, //also center
	.width = 5,
	.height = 5, // 5 by 5 size circle
	.turnDirection = 0,
	.walkDirection = 0,
	.rotationAngle = PI / 2, //90 degree in radian system
	.walkSpeed = 100, //100 pixels per sec
	.turnSpeed = 60 * (PI / 180) //45 degrees per sec
};

void movePlayer(float deltaTime)
{
	// move my player
	player.rotationAngle += player.turnDirection * player.turnSpeed * deltaTime;
	float moveStep = player.walkDirection * player.walkSpeed * deltaTime;

	float newPlayerX = player.x + cos(player.rotationAngle) * moveStep;
	float newPlayerY = player.y + sin(player.rotationAngle) * moveStep;

	//perform wall collision
	if (!mapHasWallAt(newPlayerX, newPlayerY))
	{
		player.x = newPlayerX;
		player.y = newPlayerY;

	}
}


void renderPlayer()
{
	drawRect(
		player.x * MINIMAP_SCALE_FACTOR,
		player.y * MINIMAP_SCALE_FACTOR,
		player.width * MINIMAP_SCALE_FACTOR,
		player.height * MINIMAP_SCALE_FACTOR,
		0xFFFFFFFF
	);

}