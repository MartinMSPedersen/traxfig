#include <stdio.h>
#include <stdlib.h>
#include "traxfig.h"
#include "move.h"

static void displayBoard(FILE *outP, GAME_AREA *ga) {
	int		i, j;
	int		jmin = 0;
	int		jmax = ga->height;
	int		imin = 0;
	int		imax = ga->width;

	for (j = jmin; j < jmax ; j++) {
    if (j > jmin) {
      fprintf(outP, ":");
    }
    int empty_count = 0;
		for (i = imin; i < imax ; i++) {
      switch (ga->area[ga->left_x+i][ga->up_y+j]) {
        case 0:
          if (empty_count > 0) {
            fprintf(outP, "%d", (empty_count-1));
            empty_count = 0;
          }
          fprintf(outP,"e");
          break;
        case 1:
          if (empty_count > 0) {
            fprintf(outP, "%d", (empty_count-1));
            empty_count = 0;
          }
          fprintf(outP,"b");
          break;
        case 2:
          if (empty_count > 0) {
            fprintf(outP, "%d", (empty_count-1));
            empty_count = 0;
          }
          fprintf(outP,"a");
          break;
        case 3:
          if (empty_count > 0) {
            fprintf(outP, "%d", (empty_count-1));
            empty_count = 0;
          }
          fprintf(outP,"d");
          break;
        case 4:
          if (empty_count > 0) {
            fprintf(outP, "%d", (empty_count-1));
            empty_count = 0;
          }
          fprintf(outP,"c");
          break;
        case 5:
          if (empty_count > 0) {
            fprintf(outP, "%d", (empty_count-1));
            empty_count = 0;
          }
          fprintf(outP,"f");
          break;
        case 6:
          empty_count++;
          break;
        default: 
          fprintf(outP,"%d", ga->area[i][j]);
          break;
      }
    }
  }
  fprintf(outP, "\n");
}

void uw_slugbugblueoutput(FILE *outP, GAME_AREA *ga) {
	displayBoard(outP, ga);
}
