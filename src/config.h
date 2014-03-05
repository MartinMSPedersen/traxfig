#ifndef __CONFIG
#define __CONFIG

#define MAX_FORMAT 10

typedef struct {
	char *default_format;
	char *messages_file;
} CFG_INFOS;

CFG_INFOS *cfg_init(void);
void cfg_exit(void);

#endif
