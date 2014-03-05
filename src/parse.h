/*******************************************************************\
**
\*******************************************************************/

#ifndef __PARSE
#define __PARSE

#include <stdio.h>
#include "move.h"

int ps_read_move(/* in */ int with_number, /* out */ MOVE *move, char *text);
int ps_is_move(/* in */ char word[], /* out */ MOVE *move);

#endif

