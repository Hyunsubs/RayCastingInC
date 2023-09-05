//스탠다드 라이브러리랑 SDL 라이브러리 추가
#include <stdio.h>
#include <SDL.h>

//상수 모음집
#include "constants.h"


//2차원 맵
const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


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



SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int isGameRunning = FALSE;
//이전 프레임 틱
int ticksLastFrame = 0;



int initializeWindow()
{
	//Instantiate Canvas
	//SDL INIT EVERYTHING은 SDL헤더안에 있는 상수임
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) //모든것을 초기화
	{
		fprintf(stderr, "Error initializing SDL. \n");
		return FALSE;
	}
	//에러가 없다면 0을 반환
	window = SDL_CreateWindow(
		TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr, "Error creating SDL widnow.\n");
		return FALSE;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		0
	);

	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL renderer. \n");
		return FALSE;
	}

	SDL_SetRenderDrawBlendMode(
		renderer,
		SDL_BLENDMODE_BLEND
		);

	return TRUE;
}

void destroyWindow()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

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
	player.turnSpeed = 45 * (PI / 180); //45 degrees per sec
}

int mapHasWallAt(float x, float y)
{
	if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
	{
		return TRUE;
	}
	int mapGridIndexX = floor(x / TILE_SIZE);
	int mapGridIndexY = floor(y / TILE_SIZE);
	return map[mapGridIndexY][mapGridIndexX] == 1 ? TRUE : FALSE;
}

void renderMap()
{
	for (int i = 0; i < MAP_NUM_ROWS; i++)
	{
		for (int j = 0; j < MAP_NUM_COLS; j++)
		{
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
			int tileColor = map[i][j] != 0 ? 255 : 0;

			SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
			SDL_Rect mapTileRect = {
				tileX * MINIMAP_SCALE_FACTOR,
				tileY * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
			};
			SDL_RenderFillRect(renderer, &mapTileRect);
		}
	}
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
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect playerRect = {
		player.x * MINIMAP_SCALE_FACTOR,
		player.y * MINIMAP_SCALE_FACTOR,
		player.width * MINIMAP_SCALE_FACTOR,
		player.height * MINIMAP_SCALE_FACTOR
	};
	SDL_RenderFillRect(renderer, &playerRect);

	SDL_RenderDrawLine(
		renderer,
		MINIMAP_SCALE_FACTOR * player.x,
		MINIMAP_SCALE_FACTOR * player.y,
		MINIMAP_SCALE_FACTOR * (player.x + cos(player.rotationAngle) * 40),
		MINIMAP_SCALE_FACTOR * (player.y + sin(player.rotationAngle) * 40)
	);
}


void processInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
		{
			isGameRunning = FALSE;
			break;
		}

		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				isGameRunning = FALSE;
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
	//SDL_Delay(Uint32 ms) 파라미터에 들어간 ms만큼 sleep을 수행한다.

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


}


void render()
{
	//더블 버퍼용 빈 이미지
	SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	SDL_RenderClear(renderer);


	renderMap();
	//renderRays();
	renderPlayer();


	SDL_RenderPresent(renderer);
}



int main(int argc, char* args[])
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


	destroyWindow();

	return 0;
}