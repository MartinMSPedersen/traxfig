/*******************************************************************\
**
\*******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parse.h"

/*******************************************************************/

#define MAX_BUFFER 1024

static int move_number = 1;

/*******************************************************************/

static int ps_read_word(/* out */ char word[]);
static int ps_is_number(/* in */ char *word);

static int ps_read_word(/* out */ char word[])
{	int c;
	int read = 1;
	int l = 0;

	while ((c=getchar())!= EOF&&!(isalnum(c)||c=='@'||c=='+'||c=='/'||c=='\\'));
	if (c != EOF) word[l++] = c;

	while (read && l < MAX_BUFFER && (c = getchar()) != EOF)
	{	
		read = (isalnum(c) ||c=='@' || c == '+' || c == '/' || c == '\\');
		if (read) word[l++] = c;
	}

	word[l] = 0;

	return (l > 0);
}

static int ps_is_number(/* in */ char *word)
{	int  is_number = 1;
	char *p = word;

	do
	{	is_number = isdigit(*p++);
	} while (is_number && *p != 0);

	return is_number;
}

/*-----------------------------------------------------------------*/

int ps_is_move(/* in */ char word[], /* out */ MOVE *move)
{	
	int l        = 0;
	int could_be = 1;
	enum {INIT,EXPECT_A,EXPECT_TILE,LETTERS_X,A_X,ZERO_Y,HAD_A0,NUMBERS_Y,END}
		state = INIT;

	while (could_be && l < strlen(word))
	{	
		word[l] = toupper(word[l]);

		if (state == END)
			could_be = 0;

		else if (word[l] == '@')
		{	switch(state)
			{
			case INIT:
				move->x =  0;
				state   = LETTERS_X;
            break;
			default:
				could_be = 0;
			}
		}

		else if (word[l] == '0')
		{	switch(state)
			{
         case HAD_A0:
         case ZERO_Y:
            break;
         case A_X:
				state   = HAD_A0;
            move->y = 1;
				break;
			case LETTERS_X:
				move->y = 1;
				state   = ZERO_Y;
				break;
			case NUMBERS_Y:
				move->y = move->y * 10;
				break;
			default:
				could_be = 0;
			}
		}
		else if (word[l] == '1')
		{	switch(state)
			{
			case INIT:
				move->x =  0;
				move->y = -1;
				state   = EXPECT_A;
				break;
         case ZERO_Y:
         case A_X:
         case HAD_A0:
			case LETTERS_X:
				move->y = 1;
				state   = NUMBERS_Y;
				break;
			case NUMBERS_Y:
				move->y = move->y * 10 + (int) (word[l] - '0');
				break;
			default:
				could_be = 0;
			}
		}
		else if (word[l] == 'A') 
		{	switch(state)
			{
			case EXPECT_A:
				state = EXPECT_TILE;
				break;
			case INIT:
				state   = A_X;
				move->x = 0;
				break;
         case A_X:
			case LETTERS_X:
            state = LETTERS_X;
				move->x =  ((move->x)+1) * (int) ('Z' - 'A' + 1)
     				         + (int) (word[l] - 'A');
				break;
			default:
				could_be = 0;
			}
		}
		else if (word[l] == '+' || word[l] == '\\' || word[l] == '/') 
		{	switch(state)
			{
         case HAD_A0:
            move->x =  0;
            move->y = -1;
         case ZERO_Y:
			case NUMBERS_Y:
				move->tile = (int) word[l];
				state      = END;
				break;
			default:
				could_be = 0;
			}
		}
		else if (word[l] == 'S' || word[l] == 'C' || word[l] == 'U' ||
		         word[l] == 'D' || word[l] == 'R' || word[l] == 'L') 
		{	switch(state)
			{
			case EXPECT_TILE:
			case NUMBERS_Y:
				move->tile = (int) word[l];
				state      = END;
				break;
			case INIT:
				move->x = (int) (word[l] - 'A');
				state   =  LETTERS_X;
				break;
         case A_X:
			case LETTERS_X:
            state = LETTERS_X;
				move->x =  ((move->x) + 1) * (int) ('Z' - 'A' + 1)
				         + (int) (word[l] - 'A');
				break;
			default:
				could_be = 0;
			}
		}
		else if (word[l] > 'A' && word[l] <= 'Z') 
		{	switch(state)
			{
			case INIT:
				move->x = (int) (word[l] - 'A');
				state   = LETTERS_X;
				break;
         case A_X:
			case LETTERS_X:
            state = LETTERS_X;
				move->x =  ((move->x) + 1) * (int) ('Z' - 'A' + 1)
				         + (int) (word[l] - 'A');
				break;
			default:
				could_be = 0;
			}
		}
		else if (word[l] > '0' && word[l] <= '9') 
		{	switch(state)
			{
         case ZERO_Y:
         case A_X:
			case LETTERS_X:
         case HAD_A0:
				move->y = (int) (word[l] - '0');
				state   = NUMBERS_Y;
				break;
			case NUMBERS_Y:
				move->y = move->y * 10 + (int) (word[l] - '0');
				break;
			default:
				could_be = 0;
			}
		}
		else
			could_be = 0;

		l++;
	}

	could_be = move->y != 0;

	if (move->y > 0)
		move->y--;

	return (could_be && state == END);
}

int ps_read_move(/* in */ int with_number, /* out */ MOVE *move, char *text)
{	char word[MAX_BUFFER];
	int  found = 0;
	enum {EXPECT_NUMBER, EXPECT_MOVE} state;

	state = with_number ? EXPECT_NUMBER : EXPECT_MOVE;

	while (!found && ps_read_word(word))
	{	
		if (with_number && ps_is_number(word))
		{	if (atoi(word) == move_number)
				state = EXPECT_MOVE;
		}
		else if (state == EXPECT_MOVE && ps_is_move(word, move))
		{	found = 1;
			move_number++;
		}
		else if (with_number && state == EXPECT_MOVE)
			state = EXPECT_NUMBER;
	}

	strcpy(text, word);

	return found;
}
