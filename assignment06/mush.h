#ifndef _mushy__
#define _mushy__

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
int num_pipes;
char *output;
char *input;
char *cmd_line;
char *scriptfile;
char **pline_args;
char **pline_pipeline;
int pline_argc;
bool write_prompt;
pid_t parent_id;
bool skip;

struct pipestage{
    int in;
    int out;
    char *args;

};
#endif
