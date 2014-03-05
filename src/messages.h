#ifndef __MESSAGES
#define __MESSAGES

#include <stdarg.h>

void msg_init(char *messages);
void msg_do(int msg, ...);
void msg_exit(void);

enum {
WARN_BAD_ARG,
WARN_NO_STYLE,
WARN_NO_OUTPUT,
WARN_UNKNOWN_ARG,
WARN_IGNORED,
WARN_NO_LANGUAGE,

MSG_ILLEGAL,
MSG_THREEPATHS,

MSG_USAGE,
MSG_VERSION,
MSG_MOVE,
MSG_NEWLINE,
MSG_READING,

FATAL_NO_INPUT,
NB_MESSAGES};

#endif
