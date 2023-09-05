//참 거짓 정의
#define FALSE 0
#define TRUE  1

//수학용
#define PI 3.14159265
#define TWO_PO 6.28318530

//각 셀당 타일 사이즈 64x64
#define TILE_SIZE 64
#define MAP_NUM_ROWS 13 
#define MAP_NUM_COLS 20

#define MINIMAP_SCALE_FACTOR 1.0

//호도법으로 변환해놓은 fov값
#define FOV_ANGLE (60 * PI / 180)

#define NUM_RAYS WINDOW_WIDTH

//창 정보
#define TITLE "Hyunsubs"
#define WINDOW_HEIGHT (TILE_SIZE * MAP_NUM_ROWS)
#define WINDOW_WIDTH (TILE_SIZE * MAP_NUM_COLS)


#define FPS 30

//ms단위
//what is the length of time per frame
#define FRAME_TIME_LENGTH (1000 / FPS)