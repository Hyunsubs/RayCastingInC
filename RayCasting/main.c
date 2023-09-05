//스탠다드 라이브러리랑 SDL 라이브러리 추가
#include <stdio.h>
#include <SDL.h>

//상수 모음집
#include "constants.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int isGameRunning = FALSE;

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
		}

	}
}

void render()
{
	//더블 버퍼
	SDL_SetRenderDrawColor(renderer, 255,80,80,255);
	SDL_RenderClear(renderer);


	// TODO:
	// render all game objects for the current frame
	// 모든 게임 오브젝트를 현재 프레임에 렌더


	SDL_RenderPresent(renderer);
}



int main(int argc, char* args[])
{
	//창 띄우기
	isGameRunning = initializeWindow();

	//setup();

	while (isGameRunning) 
	{
		processInput();
		//update();
		render();
	}


	destroyWindow();

	return 0;
}