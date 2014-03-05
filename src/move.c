/*******************************************************************\
**
\*******************************************************************/
#include "move.h"
#include <stdio.h>
/*******************************************************************/

enum {LEFT, RIGHT, UP, DOWN, NB_DIRECTIONS};

#define NB_MATCHING   3

#ifdef TRUE
#	undef TRUE
#endif
#ifdef FALSE
#	undef FALSE
#endif

#define TRUE      -1
#define FALSE      0

#define ABOVE      0
#define BELOW      1
#define LEFT_SIDE  0
#define RIGHT_SIDE 1

enum {U, D, L, R, NB_LETTERS};

/*-----------------------------------------------------------------*/

static GAME_AREA ga;
static int mv_nb;

static int delta_x[NB_DIRECTIONS] = {-1, 1,  0, 0};
static int delta_y[NB_DIRECTIONS] = { 0, 0, -1, 1};

static int connexions[NB_TILES][NB_DIRECTIONS][NB_MATCHING] =
{   /* VERTICAL */
	{	{VERTICAL,   LEFT_UP,    LEFT_DOWN},
		{VERTICAL,   RIGHT_DOWN, RIGHT_UP},
		{VERTICAL,   LEFT_DOWN,  RIGHT_DOWN},
		{VERTICAL,   LEFT_UP,    RIGHT_UP},
	},
	/* HORIZONTAL */
	{	{HORIZONTAL, RIGHT_DOWN, RIGHT_UP},
		{HORIZONTAL, LEFT_UP,    LEFT_DOWN},
		{HORIZONTAL, LEFT_UP,    RIGHT_UP},
		{HORIZONTAL, LEFT_DOWN,  RIGHT_DOWN},
	},
	/* LEFT_DOWN */
	{   {HORIZONTAL, RIGHT_DOWN, RIGHT_UP},
		{VERTICAL,   RIGHT_DOWN, RIGHT_UP},
		{HORIZONTAL, LEFT_UP,    RIGHT_UP},
		{VERTICAL,   LEFT_UP,    RIGHT_UP},
	},
	/* LEFT_UP */
	{	{HORIZONTAL, RIGHT_DOWN, RIGHT_UP},
		{VERTICAL,   RIGHT_DOWN, RIGHT_UP},
		{VERTICAL,   LEFT_DOWN,  RIGHT_DOWN},
		{HORIZONTAL, LEFT_DOWN,  RIGHT_DOWN},
	},
	/* RIGHT_DOWN */
	{	{VERTICAL,   LEFT_DOWN, LEFT_UP},
		{HORIZONTAL, LEFT_DOWN, LEFT_UP},
		{HORIZONTAL, RIGHT_UP,  LEFT_UP},
		{VERTICAL,   RIGHT_UP,  LEFT_UP},
	},
	/* RIGHT_UP */
	{	{LEFT_DOWN,  LEFT_UP,    VERTICAL},
		{LEFT_DOWN,  LEFT_UP,    HORIZONTAL},
		{LEFT_DOWN,  RIGHT_DOWN, VERTICAL},
		{LEFT_DOWN,  RIGHT_DOWN, HORIZONTAL},
	},
};

int translations[NB_LETTERS][2][NB_TILES] = {
	/* U */
	{	/* LEFT_SIDE */
		{LEFT_DOWN, RIGHT_UP, RIGHT_UP, RIGHT_UP, LEFT_DOWN, LEFT_DOWN},
		/* RIGHT_SIDE */
		{RIGHT_DOWN, LEFT_UP, RIGHT_DOWN, RIGHT_DOWN, LEFT_UP, LEFT_UP},
	},
	/* D */
	{	/* LEFT_SIDE */
		{LEFT_UP, RIGHT_DOWN, RIGHT_DOWN, RIGHT_DOWN, LEFT_UP, LEFT_UP},
		/* RIGHT_SIDE */
		{RIGHT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_UP, LEFT_DOWN, LEFT_DOWN},
	},
	/* L */
	{	/* ABOVE */
		{LEFT_DOWN, RIGHT_UP, RIGHT_UP, LEFT_DOWN, RIGHT_UP, LEFT_DOWN},
		/* BELOW */
		{LEFT_UP, RIGHT_DOWN, LEFT_UP, RIGHT_DOWN, LEFT_UP, RIGHT_DOWN},
	},
	/* R */
	{	/* ABOVE */
		{RIGHT_DOWN, LEFT_UP, LEFT_UP, RIGHT_DOWN, LEFT_UP, RIGHT_DOWN},
		/* BELOW */
		{RIGHT_UP, LEFT_DOWN, RIGHT_UP, LEFT_DOWN, RIGHT_UP, LEFT_DOWN},
	},
};

/*******************************************************************/

static int mv_force(int x, int y);
static int mv_connectable(MOVE *mv);static int mv_find_tile(MOVE *mv, int l, int x1, int y1, int x2, int y2);
/*  return 1 if played tile matches with neighbour tiles,
 *  0 if it doesn't match or if there isn't any neighbour tile
 */
static int mv_connectable(MOVE *mv)
{	int matching_tile;
	int connected = 0;
	int d;

	if (mv->tile == EMPTY)
		return TRUE;
	/* 1st move */
	if (ga.height == 0)
		return (mv->tile == LEFT_UP || mv->tile == VERTICAL);
	/* already played */
	if (ga.area[mv->x][mv->y] != EMPTY)
		 return FALSE;

	/*** test if matching in each direction */
	for (d = LEFT; d < NB_DIRECTIONS; d++)
	{	
		matching_tile = ga.area[mv->x + delta_x[d]][mv->y + delta_y[d]];

		if (matching_tile != EMPTY)
		{	
			/*** if does not belong to allowed connexions, failed */
			if (   matching_tile != connexions[mv->tile][d][0]
			    && matching_tile != connexions[mv->tile][d][1]
			    && matching_tile != connexions[mv->tile][d][2])
				return FALSE;

			/*** else it has a connexion */
			connected = TRUE;
		}
	}

	/* connected == FALSE, if there is no neighbour */
	return connected;
}

static int mv_force(int x, int y){	MOVE mv;
	int  nb_poss; /* number of playable tiles */
	int  legal;
	int  tile;
	/*** already played : original move still legal */
	if (ga.area[x][y] != EMPTY) return TRUE;
	/*** count number of possibilities */

	/* there should at least one playable tile as it has a neighbour
	 * else it's the case of three same colored paths entering.
	 */

	mv.x    = x;
	mv.y    = y;
	mv.tile = VERTICAL;
	nb_poss = 0;

	while (mv.tile < NB_TILES)
	{	/* count playable tiles */
		if (mv_connectable(&mv))
		{	nb_poss++;
			tile = mv.tile;
		}
		/* try next */
		mv.tile++;
	}

	legal = nb_poss > 0 ? TRUE : FALSE;

	/*** IF only one choice : play forced move */
	if (nb_poss == 1)
	{	
		ga.area[x][y] = tile;
		ga.mv_nb[x][y] = -mv_nb;
		/* area can't be extended by this move */

		/*** examinate if there's a diffusion */
		legal  = mv_force (x - 1, y); 
		legal &= mv_force (x + 1, y);
		legal &= mv_force (x, y - 1);
		legal &= mv_force (x, y + 1);
	}

	return legal;
}

static int mv_find_tile(MOVE *mv, int l, int x1, int y1, int x2, int y2)
{	mv->tile  = NB_TILES;
	if (ga.area[x1][y1] != EMPTY)
		mv->tile = translations[l][0][ga.area[x1][y1]];
	else if (ga.area[x2][y2] != EMPTY)
		mv->tile = translations[l][1][ga.area[x2][y2]];
	return (mv->tile < NB_TILES && mv_connectable(mv));
}

/*-----------------------------------------------------------------*/

void mv_init(void)
{	int x, y;

	ga.left_x = MAX_X + 1;
	ga.up_y   = MAX_Y + 1;
	ga.height = 0;
	ga.width  = 0;

	mv_nb     = 0;

	for (x = 0; x < WIDTH; x++)
	{	for (y = 0; y < HEIGHT; y++)
		{	ga.area[x][y] = EMPTY;	
			ga.mv_nb[x][y] = NOT_PLAYED;
		}
	}
}

/* play a move. This move must be connectable */
int mv_play_move(MOVE *mv)
{	int x, y;
	int legal = TRUE;

	/*** IF 1st move */
	if (ga.height == 0)
	{	ga.height = ga.width = 1;
		ga.area[ga.left_x][ga.up_y] = mv->tile;
		ga.mv_nb[ga.left_x][ga.up_y] = mv_nb = 1;
	}
	/*** NOT first move */
	else
	{	/*** play tile */
		x = mv->x; 
		y = mv->y;
		ga.area[x][y] = mv->tile;
		ga.mv_nb[x][y] = ++mv_nb;
		/*** note if area is extended */
		if (x < ga.left_x || x == (ga.left_x + ga.width))
		{	ga.width++;
			if (ga.left_x > x) ga.left_x = x;
		}
		if (y < ga.up_y || y == (ga.up_y + ga.height))
		{	ga.height++;
			if (ga.up_y > y) ga.up_y = y;
		}

		/*** play forced tiles */

		/* each direction is explored anyway, because here move
		 * can only be illegal if there are three paths of the same
		 * color entering into a square. And it's better to draw the
		 * area to show the mistake.
		 */

		legal  = mv_force (x - 1, y); 
		legal &= mv_force (x + 1, y);
		legal &= mv_force (x, y - 1);
		legal &= mv_force (x, y + 1);
	}

	return legal;
}

int mv_recognize(MOVE *mv)
{	int tile;

	/*** adapt coord */
	if (mv->x == 0 && ga.area[mv->x + ga.left_x][mv->y + ga.up_y] != EMPTY)
		mv->x--;
	else if (mv->y == 0 && ga.area[mv->x + ga.left_x][mv->y + ga.up_y] != EMPTY)
		mv->y--;
	mv->x += ga.left_x;
	mv->y += ga.up_y;

	/*** select tile */
	switch (mv->tile)
	{
	case (int) 'S':
	case (int) '+':
		mv->tile = VERTICAL;
		if (!mv_connectable(mv))
			mv->tile = HORIZONTAL;
		if (!mv_connectable(mv))
			mv->tile = NB_TILES;
		break;

	case (int) '/':
		mv->tile = LEFT_UP;
		if (!mv_connectable(mv))
			mv->tile = RIGHT_DOWN;
		if (!mv_connectable(mv))
			mv->tile = NB_TILES;
		break;

	case (int) '\\':
		mv->tile = LEFT_DOWN;
		if (!mv_connectable(mv))
			mv->tile = RIGHT_UP;
		if (!mv_connectable(mv))
			mv->tile = NB_TILES;
		break;

	case (int) 'C':
		/*** find a connectable curve */
		for (mv->tile = LEFT_DOWN; mv->tile < NB_TILES && !mv_connectable(mv); mv->tile++);

		/*** check there is no other curve */
		if (mv->tile < NB_TILES)
		{	tile = mv->tile;
			for (mv->tile++; mv->tile < NB_TILES && !mv_connectable(mv); mv->tile++);

			/*** IF there's another one : ambiguity */
			if (mv->tile < NB_TILES)		
				mv->tile = NB_TILES;
			else
				mv->tile = tile;
		}
		break;

	case (int) 'U':
		if (!mv_find_tile(mv, U, mv->x + 1, mv->y, mv->x - 1, mv->y))
			mv->tile = NB_TILES;
		break;

	case (int) 'D':
		if (!mv_find_tile(mv, D, mv->x + 1, mv->y, mv->x - 1, mv->y))
			mv->tile = NB_TILES;
		break;

	case (int) 'L':
		if (!mv_find_tile(mv, L, mv->x, mv->y + 1, mv->x, mv->y - 1))
			mv->tile = NB_TILES;
		break;

	case (int) 'R':
		if (!mv_find_tile(mv, R, mv->x, mv->y + 1, mv->x, mv->y - 1))
			mv->tile = NB_TILES;
		break;
	}

	return (mv->tile == NB_TILES);
}

GAME_AREA *mv_get_area(void)
{	return &ga;
}
