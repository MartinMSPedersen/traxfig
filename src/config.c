/*******************************************************************\
**
\*******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "traxfig.h"
#include "config.h"

/*******************************************************************/

#define DEFAULT "FORMAT "
#define MESSAGES "MESSAGES "

#define CONFIG_FILE  PATH "traxfig.cfg"

/*******************************************************************/

static CFG_INFOS cfg;

/*******************************************************************/

static char *extract(char s[]);

static char *extract(char s[])
{	size_t l = strlen(s);
	char  *b;

	while (isspace(s[--l]));
	s[l + 1] = 0;
	while (!(isspace(s[--l])));

	b = strdup(&s[++l]);

	return b; 
}

/*******************************************************************/

CFG_INFOS *cfg_init(void)
{	FILE *f_cfg;
	char  buffer[256];

	cfg.default_format = NULL;
	cfg.messages_file  = NULL;

	if ((f_cfg = fopen(CONFIG_FILE, "r")) != NULL)
	{	
		while (fgets(buffer, 256, f_cfg) != NULL)
		{	if (buffer[0] != '#')
			{
				if (strstr(buffer, DEFAULT) == buffer)
					cfg.default_format = extract(buffer);
				else if (strstr(buffer, MESSAGES) == buffer)
					cfg.messages_file = extract(buffer);
			}
		}
		fclose(f_cfg);
	}

	return &cfg;
}

void cfg_exit(void)
{	if (cfg.messages_file != NULL) free(cfg.messages_file);
	if (cfg.default_format != NULL) free(cfg.default_format);
}
