//���Ĵٵ� ���̺귯���� SDL ���̺귯�� �߰�
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <limits.h>
#include <float.h>
#include "map.h"
#include "ray.h"
#include "player.h"


//��� ������
#include "defs.h"

//sdl �׷��� ó�� ����
#include "graphics.h"

//�ؽ�ó �ϵ��ڵ� ��� �߰�
#include "textures.h"

//2���� ��








bool isGameRunning = false;
//���� ������ ƽ
int ticksLastFrame = 0;



void setup()
{
	//��� png ���ڵ��ؼ� walltextures �迭�� ��ȯ
	loadWallTexutures();
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
	//�������Ǵ� ��� ������ ���������(fps���� ���ܵǴ� ������)
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TIME_LENGTH));
	//���� ƽ�� ����ƽ(���� ������) + �ʴ� �Ѹ����� �ϴ� ������
	//�� ���� ���� ��� ������ ���� �ʴ´�.
	//�� ������ ���� ���� �������ν� �ʴ� 30�徿 �����ϰ� ��������
	//while������ �� �ִ� ������ ���μ����� �ٸ����� ���� ���ϰ� �����ִ� ���¶�� �� �� �ִ�.
	//while�� �ִ� ��� SDL DELAY�� ����Ͽ� sleep�� ��Ű�°��� ���μ��� ��뷮�� ���̴� �鿡�� ����.
	//SDL_Delay(uint32_t ms) �Ķ���Ϳ� �� ms��ŭ sleep�� �����Ѵ�.

	//��ٷ��� �� �ð� �� ������ �ð� - ���� ���� �ð�
	//���� �ž��ϴ� �ð��� �ƴ϶�� 
	//��ٷ��� �� �ð� ��ŭ ���� ������
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

		int wallStripHeight = (int)projectedWallHeight;//ray distance�� ���� ����.

		int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
		wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

		int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
		wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

		//render the wall from walltoppixel to wallbottompixel
		//õ���ȼ�
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

		//�� �ִºκ�
		for (int y = wallTopPixel; y < wallBottomPixel; y++)
		{
			// TODO: �ؽ�ó�� ������ �����ؼ� �� �ȼ� ���� �ٲ��ֱ�
			// set the color of the wall based on the color from the texture\
			// texelColor is texture pixel color
			// TODO: calculate textureOffsetY
			int distanceFromTop = y + (wallStripHeight / 2) - (WINDOW_HEIGHT / 2);
			int textureOffsetY = distanceFromTop * ((float)texture_height / wallStripHeight);

			uint32_t texelColor = wallTextures[texNum].texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
			drawPixel(x, y, texelColor);
		}

		//�ٴ� �ȼ�
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
	renderRays();
	renderPlayer();

	renderColorBuffer();
}

void releaseResources(void)
{
	freeWallTextures();
	destroyWindow();
}



int main()
{
	//â ����
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