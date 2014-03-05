/*******************************************************************\
**
\*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "messages.h"
#include "traxfig.h"

/*******************************************************************/

#define  MSGPATH PATH "msg/"
#define  DEFAULT_MSG MSGPATH "traxfig.msg"

#ifndef SEEK_END
#  define SEEK_END 2
#endif

static char *msg_buffer = NULL;
static char *msgs[NB_MESSAGES];

/*******************************************************************/

void msg_init(char *messages)
{	FILE   *f_msg;
	size_t  flen;
	int     msg       = 0;
	size_t  oldoffset = 0;
	size_t  offset    = 0;
	char    fimessages[256];

	if (messages == NULL) messages = DEFAULT_MSG;

	if ((f_msg = fopen(messages, "r")) == NULL)
	{	strcpy(fimessages, MSGPATH);
		strcat(fimessages, messages);
		if ((f_msg = fopen(fimessages, "r")) == NULL)
		{	if ((f_msg = fopen(DEFAULT_MSG, "r")) == NULL)
				exit(2);
		}
	}

	fseek(f_msg, 0L, SEEK_END);
	flen = ftell(f_msg);
	rewind(f_msg);

	if ((msg_buffer = (char *) malloc(flen + NB_MESSAGES * 6)) == NULL)
		exit(3);

	while (msg < NB_MESSAGES)
	{
		fgets(&msg_buffer[offset], flen, f_msg);

		if (strlen(&msg_buffer[offset]) == 1 && msg_buffer[offset] == '\n')
		{	msg_buffer[offset] = 0;
			offset ++;
			msgs[msg++] = &msg_buffer[oldoffset];
			oldoffset = offset;
		}
		else
		{	offset += strlen(&msg_buffer[offset]);
			if (msg_buffer[offset - 2] == '$') 
				msg_buffer[offset - 2] = 0;
		}
	}

	fclose(f_msg);
}

void msg_do(int msg, ...)
{	va_list parg;

	va_start(parg, msg);
	vfprintf(stderr, msgs[msg], parg);
	va_end(parg);

	if (msg > MSG_READING)
	{	terminate();
		fgetc(stdin);
		exit(1);
	}
}

void msg_exit(void)
{	if (msg_buffer != NULL) free(msg_buffer);
}
