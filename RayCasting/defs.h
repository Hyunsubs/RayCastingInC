#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>


//���п�
#define PI 3.14159265
#define TWO_PI 6.28318530

//�� ���� Ÿ�� ������ 64x64
#define TILE_SIZE 64

//�ؽ�ó �� ����
#define NUM_TEXTURES 9

#define MINIMAP_SCALE_FACTOR 0.2

//ȣ�������� ��ȯ�س��� fov��
#define FOV_ANGLE (60 * PI / 180)

#define NUM_RAYS WINDOW_WIDTH

#define DIST_PROJ_PLANE ((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2))


//â ����
#define TITLE "Hyunsubs"
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1280

#define FPS 60

//ms����
//what is the length of time per frame
#define FRAME_TIME_LENGTH (1000 / FPS)


#endif