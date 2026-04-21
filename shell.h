#ifndef SHELL_H
#define SHELL_H

/* === INCLUDES === */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* === PROTOTYPES - main.c === */
int process_input(const char *received_input, char **envp,
									char *shell_name, int line_number);
/* === PROTOTYPES - tokenizers.c === */
/* Public facing function to get array of substrings from a string */
char **tokenize_string(const char *string, const char *delimiters,
											 char **tokenized_string);

/* === PROTOTYPES - executioner.c === */
int execute_command(const char *command, char **arguments, char **envp);

/* === PROTOTYPES - path.c === */
char *get_cmd_fullpath(char *command, char **envp);

/* === PROTOTYPES - _getenv.c === */
char *_getenv(const char *name, char **envp);

/* === PROTOTYPES - builtins.c === */
int execute_builtin(char **envp, char **args, int last_cmd_exit_code);
#endif
