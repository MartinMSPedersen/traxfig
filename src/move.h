#ifndef __MOVE
#define __MOVE

#define MAX_X 50
#define MAX_Y 50

#define HEIGHT (MAX_Y * 2 + 1)
#define WIDTH  (MAX_X * 2 + 1)

typedef struct {
	int x;
	int y;
	int tile;
} MOVE;

typedef struct {
	int height;
	int width;
	int left_x;
	int up_y;
	int area[WIDTH][HEIGHT];
	int mv_nb[WIDTH][HEIGHT];
} GAME_AREA;

#define NOT_PLAYED 0

enum {VERTICAL, HORIZONTAL, LEFT_DOWN,  LEFT_UP, RIGHT_DOWN, RIGHT_UP, EMPTY};

#define NB_TILES EMPTY

void mv_init(void);
int mv_play_move(MOVE *mv);
int mv_recognize(MOVE *mv);
GAME_AREA *mv_get_area(void);

#endif
