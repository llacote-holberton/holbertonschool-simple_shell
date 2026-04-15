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

/* Prototype de execute_command avec envp */
void execute_command(char **args, char **envp);

/* === PROTOTYPES - path.c === */
char *get_path(char *command);

/* === PROTOTYPES - main.c === */
void free_array(char **array);

#endif
