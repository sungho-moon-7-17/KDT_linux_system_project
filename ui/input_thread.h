#ifndef _INPUT_THERAD_H
#define _INPUT_THERAD_H

#include <default.h>

#define TOY_TOK_BUFSIZE 64
#define TOY_TOK_DELIM " \t\r\n\a"

// builtin_func component func
int toy_send(char **args);
int toy_shell(char **args);
int toy_exit(char **args);

void *sensor_thread(void* arg);
void *command_thread(void* arg);

#endif