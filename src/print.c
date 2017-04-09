/*******************************************************************\
**
\*******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "traxfig.h"
#include "move.h"
#include "print.h"

/*******************************************************************/

#define MAX_TILE_HEIGHT 16
#define MAX_TILE_WIDTH  16
#define MAX_LENGTH      (MAX_TILE_WIDTH * MAX_X)
#define TFMPATH  PATH "tfm/"
#define DEFAULT_FORMAT  TFMPATH "short.tfm"

#ifndef SEEK_END
#  define SEEK_END 2
#endif

/*******************************************************************/

static int tile_height;
static int tile_width;
static char *tiles_pattern[MAX_TILE_HEIGHT * NB_TILES];
static char *fmt_buffer;

/*******************************************************************/

static char *build_empty_pattern(GAME_AREA *ga, int th, int x, int y);

#define pattern_line(tile, line) (tile * (tile_height + 1) + line)

static char *build_empty_pattern(GAME_AREA *ga, int th, int x, int y)
{	static char  buffer[MAX_TILE_WIDTH];
	int          i;

	if (th == 0 && ga->area[x][y - 1] != EMPTY)
		/* case under a tile => draw last line of the tile */	
		strcpy(buffer, tiles_pattern[pattern_line(ga->area[x][y - 1], tile_height - 1)]);
	else
	{	/* normal empty space */	
		for (i = 0; i < tile_width; buffer[i++] = ' ');
		buffer[tile_width] = 0;

		if (ga->area[x + 1][y] != EMPTY)
			buffer[tile_width - 1] = *tiles_pattern[pattern_line(ga->area[x + 1][y], th)];
		else if (th == 0 && ga->area[x][y - 1] == EMPTY && ga->area[x + 1][y - 1] != EMPTY)
			buffer[tile_width - 1] = *tiles_pattern[pattern_line(ga->area[x + 1][y - 1], tile_height - 1)];
	}


	return buffer;
}

/*******************************************************************/

void pr_init(char format[])
{	FILE   *f_fmt;
	size_t  flen;
	int     nb_line;
	int     height;
	size_t	offset;
	char    fiformat[256];

	if (format == NULL) format = DEFAULT_FORMAT;

	if ((f_fmt = fopen(format, "r")) == NULL)
	{	strcpy(fiformat, TFMPATH);
		strcat(fiformat, format);
		if ((f_fmt = fopen(fiformat, "r")) == NULL)
		{	if ((f_fmt = fopen(DEFAULT_FORMAT, "r")) == NULL)
				exit(2);
		}
	}
	fseek(f_fmt, 0L, SEEK_END);
	flen = ftell(f_fmt);
	rewind(f_fmt);

	if ((fmt_buffer = (char *) malloc(flen + MAX_TILE_HEIGHT * NB_TILES)) == NULL)
		exit(3);

	offset      = 0;
	tile_height = 0;
	height      = 0;
	nb_line     = 0;

	while (fgets(&fmt_buffer[offset], MAX_TILE_WIDTH * MAX_X + 1, f_fmt))
	{
		tiles_pattern[nb_line++] = &fmt_buffer[offset];
		height++;

		if (fmt_buffer[offset] == '\n' && tile_height == 0)
			tile_height = height - 1;			

		offset += strlen(&fmt_buffer[offset]) + 1;
	}

	while (--offset > 0)
		if (fmt_buffer[offset] == '\n') fmt_buffer[offset] = 0;
 
	tile_width = (int) strlen(fmt_buffer);

	fclose(f_fmt);
}

void pr_print_area(FILE *output, GAME_AREA *ga, int all_numbered)
{	int   x, y, w, tw, h, th /* tile height */;
	char  buffer[MAX_LENGTH];
	char  with_nb[MAX_TILE_WIDTH + 1];
	char  asc_nb[4];
	char *pattern;

	fprintf(output,"    ");
	for (w = 0; w < ga->width; w++) {
		for (tw = 0 ; tw < tile_width -1 ; tw++)
			if (tw == tile_width/2) {
				if (w + 'A' > 'Z') { 
					fprintf(output,"A%c", w+2*'A'-'Z'-1);
				} else {
					fprintf(output, "%c", w+'A');
				}
			}
			else
				if ((w + 'A' > 'Z') && ( tw == 0)) ; // skip
				else { fprintf(output, " "); }
	}
	fprintf(output,"\n");

	/*** draw area from top to bottom by row */
	for (h = 0; h < ga->height; h++)
	{	y = h + ga->up_y;

		/*** build a row */
		for (th = 0;
		     th < (tile_height - 1); /* don't draw last line */
		     th++)
		{	buffer[0] = 0;
	
			if (th == tile_height/2)
				fprintf(output," %2d ",h+1);
			else
				fprintf(output,"    ");
			for (w = 0; w < ga->width; w++)
			{	x = w + ga->left_x;

				if (ga->area[x][y] == EMPTY)
					pattern = build_empty_pattern(ga, th, x, y);
				else
					pattern = tiles_pattern[pattern_line(ga->area[x][y], th)];
				/* don't draw 1st tile's row exept for 1 row of tile */
				if (all_numbered || ga->mv_nb[x][y] > 0)	
					sprintf(asc_nb, "%3d", ga->mv_nb[x][y]);
				else
					strcpy(asc_nb, "  ");
				sprintf(with_nb, w > 0 ? pattern + 1 : pattern, asc_nb);
				strcat(buffer, with_nb);
			}

			fprintf(output, "%s\n", buffer);
		}
	}

	/*** draw last line <=> 1st line of an empty row */
	y         = ga->height + ga->up_y;
	th        = 0;
	buffer[0] = 0;

	for (w = 0; w < ga->width; w++)
	{	x       = w + ga->left_x;
		pattern = build_empty_pattern(ga, th, x, y);
		strcat(buffer, w > 0 ? pattern + 1: pattern);
	}

	fprintf(output, "    %s\n", buffer);
}

