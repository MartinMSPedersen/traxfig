/*******************************************************************\
** postscript output of a view of a trax game
** based on trax2ps, version 0.1 August 1994 Uwe G. Wilhelm
\*******************************************************************/


#include <stdio.h>
#include "traxfig.h"
#include "move.h"

/*******************************************************************/

#define TRAXSKELFILE PATH "traxSkel.ps"

static FILE * trxFp = NULL;

static char *tileps[NB_TILES] = {
	"trxV", "trxH", "trxBL", "trxTL", "trxBR", "trxTR"
};

/*******************************************************************/
static void displayBoard(FILE *outP,GAME_AREA *ga, int all_numbered);
static void displayMoves(FILE *outP, char ** inv, int inc);
static int writeSkeleton(FILE *outP);

static void displayBoard(FILE *outP,GAME_AREA *ga, int all_numbered)
{
	int             i, j;

	for (j = 0; j < ga->height ; j++) {
		for (i = 0; i < ga->width ; i++) {
			if (ga->area[ga->left_x + i][ga->up_y + j] == EMPTY) {
				/* do nothing... */ 
			} else {
				int move = ga->mv_nb[ga->left_x + i][ga->up_y + j];
				if (!all_numbered)
					move = (move > 0 ? move : 0);
				fprintf(outP, "%d %d xPos %d yPos %s\n", move, i, j, 
				          tileps[ga->area[ga->left_x + i][ga->up_y + j]]);
			}
		}
	}
}

static char *escaped (char *move)
{
	static char tmove[256];

	char *i=move;
	char *j=tmove;

	do {
		*j++ = *i;
		if (*i == '\\')
			*j++ = *i;
	} while (*i++);

	return tmove;
}

static void displayMoves(FILE *outP, char ** inv, int inc)
{
	int i;
	int xPos = 0;
	int yPos = 1;
	int xIncr = 100;


	fprintf(outP, "0 0 moveto (%s) show\n 75 0 moveto (%s) show\n",
		white_name, black_name);
	for(i = 1; i < inc; i++){
		if(i == 21){
			fprintf(outP, "175 0 moveto (%s) show\n 250 0 moveto (%s) show\n", white_name, black_name);
			xPos = 175;
			yPos = 1;
		} 
		else if(i == 41){
			fprintf(outP, "350 0 moveto (%s) show\n 425 0 moveto (%s) show\n", white_name, black_name);
			xPos = 350;
			yPos = 1;
		} 
		if(i%2){ /* odds = white */
			fprintf(outP, "%d %d moveto (%2d  %s) show\n", xPos, yPos * -16, 
						i, escaped(inv[i]));
		} else { /* evens = black */
			fprintf(outP, "%d %d moveto (%2d  %s) show\n", xPos + 75, yPos * -16, 
						i, escaped(inv[i]));
			yPos++;
		}

	}
}


static int writeSkeleton(FILE *outP)
{
	char buf[200];

	while(fgets(buf, 200, trxFp) != NULL){
		if(buf[0] == '@') break;
		fprintf(outP, "%s", buf);
	}
	return 0;
}

/*******************************************************************/


void uw_psoutput(FILE *outP, GAME_AREA *ga, char ** inv, int inc, int all_numbered)
{
	if((trxFp = fopen(TRAXSKELFILE, "r")) == NULL)
		exit(4);

	writeSkeleton(outP);

	if(ga->height <= 8 && ga->width <= 8){
		fprintf(outP, "/big\n");
	} else if(ga->height <= 10 && ga->width <= 10){
		fprintf(outP, "/medium\n");
	} else {
		fprintf(outP, "/small\n");
	}

	writeSkeleton(outP);

	displayBoard(outP, ga, all_numbered);

	writeSkeleton(outP);

	displayMoves(outP, inv, inc);

	writeSkeleton(outP);

	fclose(trxFp);
	trxFp = NULL;
}

/*******************************************************************/
