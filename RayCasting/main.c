//스탠다드 라이브러리랑 SDL 라이브러리 추가
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <limits.h>
#include <float.h>
#include "map.h"

//상수 모음집
#include "defs.h"

//sdl 그래픽 처리 관련
#include "graphics.h"

//텍스처 하드코딩 헤더 추가
#include "textures.h"

//2차원 맵



struct Player
{
	float x;
	float y;
	float width;
	float height;
	int turnDirection; // -1 for left +1 for right
	int walkDirection; // -1 for backward, +1 for forward
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;

} player;


struct Ray
{
	float rayAngle;
	float wallHitX;
	float wallHitY;
	float distance;

	bool wasHitVertical;
	int wallHitContent;
	
} rays[NUM_RAYS]; //NUM_RAYS = WINDOW_WIDTH / WALL_STRIP



bool isGameRunning = false;
//이전 프레임 틱
int ticksLastFrame = 0;



void setup()
{
	// TODO:
	// initialize and setup game objects
	player.x = WINDOW_WIDTH / 2; //center
	player.y = WINDOW_HEIGHT / 2; //also center
	player.width = 5;
	player.height = 5; // 5 by 5 size circle
	player.turnDirection = 0;
	player.walkDirection = 0;
	player.rotationAngle = PI / 2; //90 degree in radian system
	player.walkSpeed = 100; //100 pixels per sec
	player.turnSpeed = 60 * (PI / 180); //45 degrees per sec




	//모든 png 디코드해서 walltextures 배열로 변환
	loadWallTexutures();
}


float normalizeAngle(float angle)
{
	angle = remainder(angle, TWO_PI);
	if (angle < 0)
	{
		angle = TWO_PI + angle;
	}
	return angle;
}

float distanceBetweenPoints(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void castRay(float rayAngle, int stripId)
{
	rayAngle = normalizeAngle(rayAngle);

	bool isRayFacingDown = rayAngle > 0 && rayAngle < PI;
	bool isRayFacingUp = !isRayFacingDown;
	bool isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
	bool isRayFacingLeft = !isRayFacingRight;

	float xintercept, yintercept;
	float xstep, ystep;

	//HorzWallHit Part
	bool foundHorzWallHit = false;
	float horzWallHitX = 0;
	float horzWallHitY = 0;
	int horzWallContent = 0;

	yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += isRayFacingDown ? TILE_SIZE : 0;

	xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

	ystep = TILE_SIZE;
	ystep *= isRayFacingUp ? -1 : 1;

	xstep = TILE_SIZE / tan(rayAngle);
	xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

	float nextHorzTouchX = xintercept;
	float nextHorzTouchY = yintercept;

	while (
		nextHorzTouchX >= 0 &&
		nextHorzTouchX <= MAP_NUM_COLS * TILE_SIZE &&
		nextHorzTouchY >= 0 &&
		nextHorzTouchY <= MAP_NUM_ROWS * TILE_SIZE
		)
	{
		float xToCheck = nextHorzTouchX;
		float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);

		if (mapHasWallAt(xToCheck, yToCheck))
		{
			//there's a wall hit
			horzWallHitX = nextHorzTouchX;
			horzWallHitY = nextHorzTouchY;
			horzWallContent = getMapAt((int)floor(yToCheck / TILE_SIZE),(int)floor(xToCheck / TILE_SIZE));
			foundHorzWallHit = true;
			break;
		}
		else
		{
			nextHorzTouchX += xstep;
			nextHorzTouchY += ystep;
		}
	}

	//vertical part
	bool foundVertWallHit = false;
	float vertWallHitX = 0;
	float vertWallHitY = 0;
	int vertWallContent = 0;

	xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRight ? TILE_SIZE : 0;


	yintercept = player.y + (xintercept - player.x) * tan(rayAngle);

	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft ? -1 : 1;

	ystep = TILE_SIZE * tan(rayAngle);
	ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

	float nextVertTouchX = xintercept;
	float nextVertTouchY = yintercept;

	while (
		nextVertTouchX >= 0 &&
		nextVertTouchX <= MAP_NUM_COLS * TILE_SIZE &&
		nextVertTouchY >= 0 &&
		nextVertTouchY <= MAP_NUM_ROWS * TILE_SIZE
		)
	{
		float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
		float yToCheck = nextVertTouchY;

		if (mapHasWallAt(xToCheck, yToCheck))
		{
			//there's a wall hit
			vertWallHitX = nextVertTouchX;
			vertWallHitY = nextVertTouchY;
			vertWallContent = getMapAt((int)floor(yToCheck / TILE_SIZE),(int)floor(xToCheck / TILE_SIZE));
			foundVertWallHit = true;
			break;
		}
		else
		{
			nextVertTouchX += xstep;
			nextVertTouchY += ystep;
		}
	}


	//Calculate both horizontal and vertical hit distances and choose the smallest one
	float horzHitDistance = foundHorzWallHit 
		? distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY)
		: FLT_MAX;
	float vertHitDistance = foundVertWallHit 
		? distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY) 
		: FLT_MAX;

	if (vertHitDistance < horzHitDistance) 
	{
		rays[stripId].distance = vertHitDistance;
		rays[stripId].wallHitX = vertWallHitX;
		rays[stripId].wallHitY = vertWallHitY;
		rays[stripId].wallHitContent = vertWallContent;
		rays[stripId].wasHitVertical = true;
		rays[stripId].rayAngle = rayAngle;
	}
	else
	{
		rays[stripId].distance = horzHitDistance;
		rays[stripId].wallHitX = horzWallHitX;
		rays[stripId].wallHitY = horzWallHitY;
		rays[stripId].wallHitContent = horzWallContent;
		rays[stripId].wasHitVertical = false;
		rays[stripId].rayAngle = rayAngle;
	}


}

void castAllRays() 
{
	////start first ray subracting half of our FOV
	//float rayAngle = player.rotationAngle - (FOV_ANGLE / 2);
	//
	//왼쪽부터 시작하는 개념을 없애고 시작함
	for (int col = 0; col < NUM_RAYS; col++)
	{
		float rayAngle = player.rotationAngle + atan((col - NUM_RAYS / 2) / DIST_PROJ_PLANE);
		castRay(rayAngle, col);
		
	}
}



void renderRays() {
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//for (int i = 0; i < NUM_RAYS; i++)
	//{
	//	SDL_RenderDrawLine(
	//		renderer,
	//		MINIMAP_SCALE_FACTOR * player.x,
	//		MINIMAP_SCALE_FACTOR * player.y,
	//		MINIMAP_SCALE_FACTOR * rays[i].wallHitX,
	//		MINIMAP_SCALE_FACTOR * rays[i].wallHitY
	//	);
	//}
}

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
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//SDL_Rect playerRect = {
	//	player.x * MINIMAP_SCALE_FACTOR,
	//	player.y * MINIMAP_SCALE_FACTOR,
	//	player.width * MINIMAP_SCALE_FACTOR,
	//	player.height * MINIMAP_SCALE_FACTOR
	//};
	//SDL_RenderFillRect(renderer, &playerRect);

	//SDL_RenderDrawLine(
	//	renderer,
	//	MINIMAP_SCALE_FACTOR * player.x,
	//	MINIMAP_SCALE_FACTOR * player.y,
	//	MINIMAP_SCALE_FACTOR * (player.x + cos(player.rotationAngle) * 40),
	//	MINIMAP_SCALE_FACTOR * (player.y + sin(player.rotationAngle) * 40)
	//);



}


void processInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
		{
			isGameRunning = false;
			break;
		}

		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				isGameRunning = false;
				break;
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				player.walkDirection = +1;
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				player.walkDirection = -1;
			}
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				player.turnDirection = -1;
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				player.turnDirection = +1;
			}
			break;
		}

		case SDL_KEYUP: {
			if (event.key.keysym.sym == SDLK_UP)
			{
				player.walkDirection = 0;
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				player.walkDirection = 0;
			}
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				player.turnDirection = 0;
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				player.turnDirection = 0;
			}
			break;
		}

	}
}

void update()
{
	//waste some time until we reach the target frame time
	//렌더링되는 몇몇 순간은 버려줘야함(fps에서 제외되는 프레임)
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TIME_LENGTH));
	//현재 틱이 이전틱(이전 프레임) + 초당 뿌리고자 하는 프레임
	//을 넘지 않은 경우 렌더를 하지 않는다.
	//이 조건을 렌더 하지 않음으로써 초당 30장씩 균일하게 렌더링됨
	//while루프에 들어가 있는 동안은 프로세서가 다른것을 하지 못하고 갇혀있는 상태라고 볼 수 있다.
	//while에 넣는 대신 SDL DELAY를 사용하여 sleep을 시키는것이 프로세서 사용량을 줄이는 면에서 좋다.
	//SDL_Delay(uint32_t ms) 파라미터에 들어간 ms만큼 sleep을 수행한다.

	//기다려야 할 시간 각 프레임 시간 - 현재 렌더 시간
	//렌더 돼야하는 시간이 아니라면 
	//기다려야 할 시간 만큼 슬립 수행함
	int timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);

	if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH)
	{
		SDL_Delay(timeToWait);
	}

	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

	ticksLastFrame = SDL_GetTicks();

	movePlayer(deltaTime);
	castAllRays();

}

void renderWallProjection()
{
	for (int x = 0; x < NUM_RAYS; x++)
	{
		float perpDistance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle);
		float projectedWallHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;

		int wallStripHeight = (int)projectedWallHeight;//ray distance와 관련 있음.

		int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
		wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

		int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
		wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

		//render the wall from walltoppixel to wallbottompixel
		//천장픽셀
		for (int y = 0; y < wallTopPixel; y++)
		{
			drawPixel(x,y, 0xFF444444);
		}

		int textureOffsetX;
		//TODO: calculate textureOffsetX
		if (rays[x].wasHitVertical) 
		{
			//perform offset for the vertical hit
			textureOffsetX = (int)(rays[x].wallHitY) % TILE_SIZE;
		}
		else
		{
			textureOffsetX = (int)(rays[x].wallHitX) % TILE_SIZE;
			//perform offset for the horizontal hit
		}

		//get the correct texture id number from the map content
		int texNum = rays[x].wallHitContent - 1;
		
		int texture_width = wallTextures[texNum].width;
		int texture_height = wallTextures[texNum].height;

		//벽 있는부분
		for (int y = wallTopPixel; y < wallBottomPixel; y++)
		{
			// TODO: 텍스처에 색깔을 참조해서 벽 픽셀 색을 바꿔주기
			// set the color of the wall based on the color from the texture\
			// texelColor is texture pixel color
			// TODO: calculate textureOffsetY
			int distanceFromTop = y + (wallStripHeight / 2) - (WINDOW_HEIGHT / 2);
			int textureOffsetY = distanceFromTop * ((float)texture_height / wallStripHeight);

			uint32_t texelColor = wallTextures[texNum].texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
			drawPixel(x, y, texelColor);
		}

		//바닥 픽셀
		for (int y = wallBottomPixel + 1; y < WINDOW_HEIGHT; y++)
		{
			drawPixel(x, y, 0xFF888888);
		}

	}
}



void render()
{
	clearColorBuffer(0xFF000000);
	// TODO : 3d projection of walls
	renderWallProjection();
	//drawRect(100, 200, 500, 300, 0xFF0F0F0F);
	//display the minimap
	renderMap();
	//renderRays();
	//renderPlayer();

	renderColorBuffer();
}

void releaseResources(void)
{
	freeWallTextures();
	destroyWindow();
}



int main()
{
	//창 띄우기
	isGameRunning = initializeWindow();

	setup();

	while (isGameRunning) 
	{
		processInput();
		update();
		render();
	}


	releaseResources();

	return 0;
}