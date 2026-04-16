#ifndef SHELL_H
#define SHELL_H

/* Prototypes file per file (add a section for each file) */

/* === INCLUDES === */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

char **tokenize_string(const char *string, const char *delimiters);


/* === PROTOTYPES - path.c === */
char *get_cmd_fullpath(char *command, char **envp);

/* === PROTOTYPES - env_utils.c === */
char *_getenv(const char *name, char **envp);

/* === PROTOTYPES - main.c === */
void free_array(char **array);

#endif
