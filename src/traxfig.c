/*******************************************************************\
**
\*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "traxfig.h"
#include "messages.h"
#include "move.h"
#include "parse.h"
#include "config.h"
#include "print.h"
#include "psoutput.h"
#include "htmloutput.h"

/*******************************************************************/

#define DEFAULT_OUTPUT "traxfig.out"

#define NONE             0
#define VERBOSE          1
#define HELP            (1<<1)
#define OUTPUT2FILE     (1<<2)
#define OUTPUT2DEFAULT  (1<<3)
#define CUSTOM_TILES    (1<<4)
#define INPUT_FROM_FILE (1<<5)
#define PRINT_USAGE     (1<<6)
#define NUMBER_AND_MOVE (1<<7)
#define POSTSCRIPT      (1<<8)
#define ALL_NUMBERED    (1<<9)
#define HTML            (1<<10)
#define IS_INTERACTIVE  (1<<11)
#define HTML_INIT       (1<<12)
#define HAS_TABLE       (1<<13)

#define MAX_LEN_MOVE    10
#define MAX_MOVE        80

/*-----------------------------------------------------------------*/

char *output_file_name;
char *input_file_name = "stdin";
char *custom_tiles;

char *moves[MAX_MOVE];
int  nb_moves = 1;

FILE *f_in  = NULL;
FILE *f_out = NULL;

char *game_name = "of Trax";
char *white_name = "White";
char *black_name = "Black";

/*******************************************************************/

#define is_html() (getenv("QUERY_STRING") != NULL)

static char *fake_argv[1000]; /* Shuld be sufficient? */
static char *query = NULL;

int do_html_switches(/* in/out */ int *arg, /* in/out */ int *argc,
		     /* in/out */ char ***argv)
{
	int i, j, k;
	int flags;
	flags = HTML | IS_INTERACTIVE;
	query = strdup(getenv("QUERY_STRING"));
	for (i = j = 0; query[i]; i++) {
		switch (query[i]) {
		case '%':
			i++;
			k = query[i]<='9' ? query[i]-'0' :
			  query[i]<='F'? query[i]-'A'+10 :
			  query[i]-'a'+10 ;
			query[j] = 16U*(unsigned char)(unsigned)k;
			i++;
			k = query[i]<='9' ? query[i]-'0' :
			  query[i]<='F'? query[i]-'A'+10 :
			  query[i]-'a'+10 ;
			query[j++] += (unsigned char)(unsigned)k;
			break;
		case '+':
			query[j++] = ' ';
			break;
		default:
			query[j++] = query[i];
			break;
		}
	}
	query[j]='\0';
	(*argv) = fake_argv;;
	(*argc) = 1;
	for (i = 0; query[i]; i++) {
		int sep = (query[i]=='=' || ((*argc)>4 && query[i]==' '));
		if (sep) {
			(*argv)[(*argc)++] = query+i+1;
			fprintf(stderr, ":::%d %s %s\n",(*argc)-1,
				(*argv)[(*argc)-1],query+i+1);
		}
		if (query[i]=='&' || sep)
			query[i]='\0';
	}
	if ((*argc) > 3) {
		game_name = (*argv)[1];
		white_name = (*argv)[2];
		black_name = (*argv)[3];
		if (strcasecmp((*argv)[4],"yes")==0) flags |= HAS_TABLE;
		(*arg) = 5;
	} else {
		(*arg) = 1;
		flags |= HTML_INIT;
	}
	return flags;
}

/*******************************************************************/

static int do_switches(/* in/out */ int *argc, /* in */ char *argv[]);

static int do_switches(/* in/out */ int *argc, /* in */ char *argv[])
{	int    arg = 1;
	int    flags = NONE;
	MOVE   move;

	while (arg < *argc)
	{	
		switch (argv[arg][0])
		{
		case '-':
			if (strlen(argv[arg]) != 2 && strcmp("-ps", argv[arg]))
				msg_do(WARN_BAD_ARG, argv[arg]);
			else
			{	switch (argv[arg][1])
				{
				case 'f':
					if (++arg < *argc)
					{	flags       |= CUSTOM_TILES;
						custom_tiles = argv[arg];
					}
					else
						msg_do(WARN_NO_STYLE);
					break;
				case 'l':
					if (++arg < *argc)
					{	msg_exit();
						msg_init(argv[arg]);
					}
					else
						msg_do(WARN_NO_STYLE);
					break;
				case 'o':
					if (++arg < *argc)
					{	if (argv[arg][0] == '=')
						{	flags |= OUTPUT2DEFAULT;
							output_file_name = DEFAULT_OUTPUT;
						}
						else
						{	output_file_name  = argv[arg];
							flags            |= OUTPUT2FILE;
						}
					}
					else
						msg_do(WARN_NO_OUTPUT);
					break;
				case 'v':
					flags |= VERBOSE;
					break;
				case 'h':
					flags |= PRINT_USAGE;
					break;
				case 'n':
					flags |= NUMBER_AND_MOVE;
					break;
				case 'p':
					flags |= POSTSCRIPT;
					break;
				case 'a':
					flags |= ALL_NUMBERED;
					break;
				case 'u':
					flags |= HTML;
					break;
				case 'i':
					flags |= IS_INTERACTIVE;
					break;
				case 't':
					flags |= HAS_TABLE;
					break;
				case 'b':
					if (++arg < *argc)
						black_name = argv[arg];
					else
						msg_do(WARN_NO_STYLE);
					break;
				case 'w':
					if (++arg < *argc)
						white_name = argv[arg];
					else
						msg_do(WARN_NO_STYLE);
					break;
				case 'g':
					if (++arg < *argc)
						game_name = argv[arg];
					else
						msg_do(WARN_NO_STYLE);
					break;
				default:
					msg_do(WARN_UNKNOWN_ARG, argv[arg]);
					break;
				}
			}
			break;
		case '?':
		case 'h':
			flags |= PRINT_USAGE;
			break;
		default:
			if (ps_is_move(strdup(argv[arg]), &move))
			{	int tmp   = arg;
				arg       = *argc;
				*argc     = tmp;
			}
			else
			{	flags |= INPUT_FROM_FILE;
				input_file_name = argv[arg];


				while (++arg < *argc)
					msg_do(WARN_IGNORED, argv[arg]);
			}
		}
		arg ++;
	}

	/* no other comments if postcript output */
	if (flags & (POSTSCRIPT || HTML)) flags &= ~VERBOSE;

	return flags;
}

/*-----------------------------------------------------------------*/

void terminate(void)
{	if (f_out != NULL) fclose(f_out);
	if (f_in != NULL && f_in != stdin) fclose(f_in);
	msg_exit();
	cfg_exit();
	if (query != NULL)
		free(query);
#ifdef ATARI
	fgetc(stdin);
#endif
}

int main(int argc, char *argv[])
{	int        arg   = argc;
	int        flags;
	MOVE       move;
	char       text[MAX_LEN_MOVE];
	CFG_INFOS *cfg;

	/*** several init. */
	cfg = cfg_init();

	msg_init(cfg->messages_file);
	mv_init();

	if (is_html())
		flags = do_html_switches(&arg, &argc, &argv);
	else
		flags = do_switches(&arg, argv);


	if (flags & PRINT_USAGE)
	{	msg_do(MSG_USAGE);
		exit(0);
	}

	if (flags & VERBOSE)
		msg_do(MSG_VERSION);

	/*** build view */
	if (arg < argc)
	{	/*** from arguments given to the program */
		while (arg < argc)
		{	if (flags & VERBOSE)
				msg_do(MSG_MOVE, argv[arg]);

			if (ps_is_move(argv[arg], &move))
			{	
				if (mv_recognize(&move))
					msg_do(MSG_ILLEGAL, argv[arg]); 
				else if (mv_play_move(&move) == 0)
				{	msg_do(MSG_THREEPATHS, argv[arg]);
					break;
				}
				else
					moves[nb_moves++] = strdup(argv[arg]);
			}

			arg++;
		}
	}
	else if (!(flags & HTML))
	{	if (flags & INPUT_FROM_FILE)
		{	/*** from file */
			if ((f_in = stdin) == NULL)
				msg_do(FATAL_NO_INPUT, input_file_name);
		}

		if (flags & VERBOSE)
			msg_do(MSG_READING, input_file_name); 
		
		while (ps_read_move(flags & NUMBER_AND_MOVE, &move, text))
		{	if (flags & VERBOSE)
				msg_do(MSG_MOVE, text); 

			if (mv_recognize(&move))
				msg_do(MSG_ILLEGAL, text); 
			else if (mv_play_move(&move) == 0)
			{	msg_do(MSG_THREEPATHS, text);
				break;
			}
			else
				moves[nb_moves++] = strdup(text);
	
		}

		if (f_in != NULL && f_in != stdin) fclose(f_in);
		f_in = NULL;
	}

	if (flags & VERBOSE)
		msg_do(MSG_NEWLINE);

	f_out = stdout;
	if (flags & (OUTPUT2FILE|OUTPUT2DEFAULT))
		f_out = fopen(output_file_name, "w");

	
	if (flags & POSTSCRIPT)
		uw_psoutput(f_out, mv_get_area(), moves, nb_moves, flags & ALL_NUMBERED);
	else if (flags & HTML)
		uw_htmloutput(f_out, mv_get_area(), moves, nb_moves, flags & IS_INTERACTIVE,
                              flags & HAS_TABLE, flags & HTML_INIT);
	else
	{
		pr_init(flags & CUSTOM_TILES ? custom_tiles : cfg->default_format);		
		pr_print_area(f_out, mv_get_area(), flags & ALL_NUMBERED);
	}

	if (flags & (OUTPUT2FILE|OUTPUT2DEFAULT))
	{	fclose(f_out);
		f_out = NULL;
		if ((flags & INPUT_FROM_FILE) && (flags & OUTPUT2DEFAULT))
		{	remove(input_file_name);
			rename(output_file_name, input_file_name);
		}
	}

	terminate();

	return 0;
}
