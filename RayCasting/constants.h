//�� ���� ����
#define FALSE 0
#define TRUE  1

//���п�
#define PI 3.14159265
#define TWO_PO 6.28318530

//�� ���� Ÿ�� ������ 64x64
#define TILE_SIZE 64
#define MAP_NUM_ROWS 13 
#define MAP_NUM_COLS 20

#define MINIMAP_SCALE_FACTOR 1.0

//ȣ�������� ��ȯ�س��� fov��
#define FOV_ANGLE (60 * PI / 180)

#define NUM_RAYS WINDOW_WIDTH

//â ����
#define TITLE "Hyunsubs"
#define WINDOW_HEIGHT (TILE_SIZE * MAP_NUM_ROWS)
#define WINDOW_WIDTH (TILE_SIZE * MAP_NUM_COLS)


#define FPS 30

//ms����
//what is the length of time per frame
#define FRAME_TIME_LENGTH (1000 / FPS)