#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>


//수학용
#define PI 3.14159265
#define TWO_PI 6.28318530

//각 셀당 타일 사이즈 64x64
#define TILE_SIZE 64

//텍스처 총 갯수
#define NUM_TEXTURES 9

#define MINIMAP_SCALE_FACTOR 0.2

//호도법으로 변환해놓은 fov값
#define FOV_ANGLE (60 * PI / 180)

#define NUM_RAYS WINDOW_WIDTH

#define DIST_PROJ_PLANE ((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2))


//창 정보
#define TITLE "Hyunsubs"
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1280

#define FPS 60

//ms단위
//what is the length of time per frame
#define FRAME_TIME_LENGTH (1000 / FPS)


#endif