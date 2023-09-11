#include "graphics.h"



static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static uint32_t* colorBuffer = NULL;
static SDL_Texture* colorBufferTexture = NULL;

bool initializeWindow()
{
	//Instantiate Canvas
	//SDL INIT EVERYTHING은 SDL헤더안에 있는 상수임
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) //모든것을 초기화
	{
		fprintf(stderr, "Error initializing SDL. \n");
		return false;
	}

	//현재 디스플레이 해상도 구하기
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	int fullScreenWidth = display_mode.w;
	int fullScreenHeight = display_mode.h;

	//에러가 없다면 0을 반환
	window = SDL_CreateWindow(
		TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		fullScreenWidth,
		fullScreenHeight,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr, "Error creating SDL widnow.\n");
		return false;
	}


	renderer = SDL_CreateRenderer(
		window,
		-1,
		0
	);

	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL renderer. \n");
		return false;
	}

	SDL_SetRenderDrawBlendMode(
		renderer,
		SDL_BLENDMODE_BLEND
	);

	//화면 사이즈만큼 컬러버퍼 메모리 할당 
	colorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * (uint32_t)WINDOW_WIDTH * (uint32_t)WINDOW_HEIGHT);

	colorBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);

	return true;
}


void destroyWindow()
{
	// TODO: Free all textures
	free(colorBuffer);
	SDL_DestroyTexture(colorBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void clearColorBuffer(uint32_t color)
{
	for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++)
	{
		colorBuffer[i] = color;
	}
}

void renderColorBuffer()
{
	//pitch 는 texture per row임
	SDL_UpdateTexture
	(
		colorBufferTexture,
		NULL,
		colorBuffer,
		(int)((uint32_t)WINDOW_WIDTH * sizeof(uint32_t))
	);

	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void drawPixel(int x, int y, uint32_t color)
{
	colorBuffer[(WINDOW_WIDTH * y) + x] = color;

}

void drawRect(int x, int y, int width, int height, uint32_t color)
{
	
	for (int i = x; i <= (x + width); i++)
	{
		for (int j = y; j <= (y + height); j++)
		{
			drawPixel(i, j, color);
		}
	}
}


//DDA 알고리즘 활용
void drawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
	//x y 변화값 구하기
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	//변화값이 더 큰 요소를 기준 length로 사용하기
	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);


	//x값 증가량 = 1 y값 증가량 = y 변화값 / x 변화값
	//x를 기준으로 정했기 때문에 x가 1씩 증가할때마다 y가 특정값 증가함
	//division은 항상 floating point number를 발생시키기 때문에
	//이를 더 현명하게 해결한 브레슨햄 알고리즘을 사용하는 것이 좋다
	float x_inc = delta_x / (float)side_length;
	float y_inc = delta_y / (float)side_length;

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i < side_length; i++)
	{
		//픽셀의 좌표이기 때문에 반올림으로 값을 정수형으로 바꿔줘야함
		drawPixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}

}