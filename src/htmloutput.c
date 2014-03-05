/*******************************************************************\
** html output of a view of a trax game
** written by Soren Pingel Dalsgaard, March 1995
\*******************************************************************/


#include <stdio.h>
#include "traxfig.h"
#include "move.h"

/*******************************************************************/

#define TRAXSKELFILE PATH "traxSkel.html"

static FILE * trxFp = NULL;

#define IMGPATH "img/"

static char *tilehtml[NB_TILES+1] = {
	IMGPATH "trxV",
	IMGPATH "trxH",
	IMGPATH "trxBL",
	IMGPATH "trxTL",
	IMGPATH "trxBR",
	IMGPATH "trxTR",
	IMGPATH "trx"
};

#define SMALL ".medium"
/*#define SMALL ".small"*/
#define MEDIUM ".medium"
#define BIG ".big"
#define GIF ".gif"

#define max(a,b) ((a)>(b)?(a):(b))

/*******************************************************************/
static void displayTitle(FILE *outP, int init);
static void displayHeader(FILE *outP, int init);
static void displayBoard(FILE *outP,GAME_AREA *ga, int has_table);
static void displayMoves(FILE *outP, char ** inv, int inc, int has_table, int init);
static void displayForm(FILE *outP, char ** inv, int inc, int has_table, int init);
static int writeSkeleton(FILE *outP);

static void displayTitle(FILE *outP, int init)
{
	if (init)
		fprintf(outP, "New game of Trax");
	else
		fprintf(outP, "Game %s	-  %s vs. %s\n",
			game_name, white_name, black_name);
}

static void displayHeader(FILE *outP, int init)
{
	if (init)
		fprintf(outP, "Trax\n", game_name);
	else
		fprintf(outP, "Game %s\n", game_name);
}

static void displayBoard(FILE *outP, GAME_AREA *ga, int has_table)
{
	int		i, j;
	char		*sizename;
	int		size;
	int		jmin = 0;
	int		jmax = ga->height;
	int		imin = 0;
	int		imax = ga->width;

	size = max(ga->height, ga->width);
	if (size <= 8) sizename = BIG;
	else if (size < 12) sizename = MEDIUM;
	else sizename = SMALL;

	if (has_table) {
		fprintf(outP, "<TABLE>\n");
		imin--; imax++;
		jmin--; jmax++;
	}
	for (j = jmin; j < jmax ; j++) {
		if (has_table)
			fprintf(outP, "<TR>");
		for (i = imin; i < imax ; i++) {
			if (has_table)
				fprintf(outP, "<TH>");
			if (has_table &&
			    ga->area[ga->left_x + i][ga->up_y + j]==EMPTY) {
				int up,down,left,right,count;
				char pos[10];
				/* Check if we should print tile position
				   help */
				left = ga->area[ga->left_x+i-1][ga->up_y+j];
				right = ga->area[ga->left_x+i+1][ga->up_y+j];
				up = ga->area[ga->left_x+i][ga->up_y+j-1];
				down = ga->area[ga->left_x+i][ga->up_y+j+1];
				count = 0;
				if (left != EMPTY) count++;
				if (right != EMPTY) count++;
				if (up != EMPTY) count++;
				if (down != EMPTY) count++;
				if (count > 0) {
					int tu,td,tl,tr,ts,tc;
					if (i==0 && j<0)
						sprintf(pos, "1A");
					else {
						int ii=i+(i<0?1:0);
						int jj=j+1+(j<0?1:0);
						if (ii<26)
							sprintf(pos, "%c%d",
								'A'+ii, jj);
						else
							sprintf(pos,
								"%c%c%d",
								'A'+ii/26-1,
								'A'+i%26,
								jj);
					}
					tu=td=tl=tr=ts=tc=1;
					if (down != EMPTY || up != EMPTY)
						tu = td = 0;
					if (left != EMPTY || right != EMPTY)
						tl = tr = 0;
					if (count == 1) tc = 0;
#if 0					
					if (tl) fprintf(outP, "%sL<BR>",pos);
					if (tr) fprintf(outP, "%sR<BR>",pos);
					if (tu) fprintf(outP, "%sU<BR>",pos);
					if (td) fprintf(outP, "%sD<BR>",pos);
					if (tc) fprintf(outP, "%sC<BR>",pos);
					if (ts) fprintf(outP, "%sS<BR>",pos);
#else
					fprintf(outP, "%s", pos);
#endif
				}
			} else
				fprintf(outP, "<IMG SRC=\"%s%s" GIF "\">",
					tilehtml[ga->area[ga->left_x + i]
						[ga->up_y + j]],
					sizename);
			if (has_table)
				fprintf(outP, "</TH>");
		}
		if (has_table)
			fprintf(outP, "</TR>\n");
		else
			fprintf(outP, "<BR>\n");
	}
	if (has_table)
		fprintf(outP, "</TABLE>\n");
}

static void displayMoves(FILE *outP, char ** inv, int inc, int has_table,
			 int init)
{
	int i;

	if (init) return;

	if (has_table) {
		fprintf(outP, "<TABLE BORDER>\n<TR><TH COLSPAN=2>%s</TH><TH COLSPAN=2>%s</TH></TR>\n", white_name, black_name);
		for(i = 1; i < inc; i++){
			if(i%2){ /* odds = white */
				fprintf(outP, "<TR><TD>%d</TD><TD>%s</TD>", i, inv[i]);
			} else { /* evens = black */
				fprintf(outP, "<TD>%d</TD><TD>%s</TD></TR>\n", i, inv[i]);
			}
		}
		if (!(inc%2))
			fprintf(outP, "<TD></TD><TD></TD></TR>\n");
		fprintf(outP, "</TABLE>\n");
	} else {
		fprintf(outP, "<PRE>\n %-8s    %s", white_name, black_name);
		for(i = 1; i < inc; i++){
			if(i%2){ /* odds = white */
				fprintf(outP, "\n%3d %-5s", i, inv[i]);
			} else { /* evens = black */
				fprintf(outP, "	  %3d %s", i, inv[i]);
			}
		}
		fprintf(outP, "\n</PRE>\n");
	}
}


static void displayForm(FILE *outP, char ** inv, int inc, int has_table, int init)
{
	int i;

	fprintf(outP, "<FORM ACTION=\"traxfig.exe?\" METHOD=GET>\n");
	if (init) {
		fprintf(outP,
			"This will be: Game "
			"<INPUT NAME=\"G\" TYPE=TEXT VALUE=\"%s\"><BR>\n",
			game_name);
		fprintf(outP,
			"Please enter name: "
			"<INPUT NAME=\"W\" TYPE=TEXT VALUE=\"%s\"><BR>\n",
			white_name);
		fprintf(outP,
			"Please enter name: "
			"<INPUT NAME=\"B\" TYPE=TEXT VALUE=\"%s\"><BR>\n",
			black_name);
		fprintf(outP,
			"My viewer supports tables: "
			"<SELECT NAME=\"T\">"
			"<OPTION>No<OPTION>Yes"
			"</SELECT><BR>\n");
	} else {
		fprintf(outP,
			"<INPUT NAME=\"G\" TYPE=HIDDEN VALUE=\"%s\">\n",
			game_name);
		fprintf(outP,
			"<INPUT NAME=\"W\" TYPE=HIDDEN VALUE=\"%s\">\n",
			white_name);
		fprintf(outP,
			"<INPUT NAME=\"B\" TYPE=HIDDEN VALUE=\"%s\">\n",
			black_name);
		fprintf(outP,
			"<INPUT NAME=\"T\" TYPE=HIDDEN VALUE=\"%s\">\n",
			has_table ? "yes" : "no");
	}
	for (i = 1; i < inc; i++)
		fprintf(outP, "<INPUT NAME=\"%d\" TYPE=HIDDEN VALUE=\"%s\">\n",
			i, inv[i]);
	fprintf(outP, "Enter move #%d: <INPUT NAME=\"%d\" TYPE=TEXT>\n", i, i);
	fprintf(outP, "<INPUT VALUE=\"OK\" TYPE=SUBMIT>\n");
	fprintf(outP, "</FORM>\n\n");

	if (inc < 2) return;
#if 0
	fprintf(outP, "<FORM ACTION=\"http://www.daimi.aau.dk/htbin/form2mail.perl\" METHOD=POST>\n");
	fprintf(outP, "<INPUT NAME=\"pingel@daimi.aau.dk\" TYPE=HIDDEN VALUE=\"MailFromWWW\">\n");
	fprintf(outP, "<INPUT NAME=\"subject\" TYPE=HIDDEN VALUE=\"Test Document\">\n");
	fprintf(outP, "<INPUT NAME=\"message\" TYPE=HIDDEN VALUE=\"Last move was %s\">\n", inv[inc-1]);
	fprintf(outP, "username: <INPUT NAME=\"address\" TYPE=TEXT VALUE=\"Username\">\n");
	fprintf(outP, "<INPUT VALUE=\"I\'ll kill you if you push this button!\" TYPE=SUBMIT>\n");
	fprintf(outP, "</FORM>\n");
#endif
	fprintf(outP, "<FORM ACTION=\"traxfig.exe?\" METHOD=GET>\n");
	fprintf(outP, "<INPUT NAME=\"G\" TYPE=HIDDEN VALUE=\"%s\">\n",
		game_name);
	fprintf(outP, "<INPUT NAME=\"W\" TYPE=HIDDEN VALUE=\"%s\">\n",
		white_name);
	fprintf(outP, "<INPUT NAME=\"B\" TYPE=HIDDEN VALUE=\"%s\">\n",
		black_name);
	fprintf(outP, "<INPUT NAME=\"T\" TYPE=HIDDEN VALUE=\"%s\">\n",
		has_table ? "yes" : "no");
	for (i = 1; i < inc-1; i++)
		fprintf(outP, "<INPUT NAME=\"%d\" TYPE=HIDDEN VALUE=\"%s\">\n",
			i, inv[i]);
	fprintf(outP, "<INPUT VALUE=\"Cancel\" TYPE=SUBMIT>\n");
	fprintf(outP, "</FORM>\n");
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


void uw_htmloutput(FILE *outP, GAME_AREA *ga, char ** inv, int inc, int is_interactive, int has_table, int init)
{
	if((trxFp = fopen(TRAXSKELFILE, "r")) == NULL)
		exit(4);

	if (is_interactive)
		fprintf(outP, "Content-Type: text/html\n\n");

	writeSkeleton(outP);
	displayTitle(outP, init);
	writeSkeleton(outP);
	displayHeader(outP, init);
	writeSkeleton(outP);
	displayMoves(outP, inv, inc, has_table, init);
	writeSkeleton(outP);
	displayBoard(outP, ga, has_table);
	writeSkeleton(outP);
	displayForm(outP, inv, inc, has_table, init);
	writeSkeleton(outP);

	fclose(trxFp);
	trxFp = NULL;
}

/*******************************************************************/

