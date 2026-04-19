#ifndef SHELL_H
#define SHELL_H

/* === PROTOTYPES - main.c === */
int process_input(const char *received_input, char **envp);
/* @note: put here at least while we develop to allow "unit tests" */

/* === PROTOTYPES - tokenizers.c === */
/* Public facing function to get array of substrings from a string */
char **tokenize_string(const char *string, char *delimiters,
											 char **tokenized_string);

/* === PROTOTYPES - executioner.c === */
int execute_command(const char *command, char **arguments, char **envp);

/* === PROTOTYPES - error_manager.c*/
char *get_set_program_name(char *string);
unsigned int get_set_currentline_number(int increment);
void log_internal_error(char *func_name);
void log_functional_error(char *cmd_tried);
void *log_error(char *error_code, char *func_name, char *cmd_tried);

/* === INCLUDES === */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>


/* === PROTOTYPES - path.c === */
char *get_cmd_fullpath(char *command, char **envp);

/* === PROTOTYPES - env_utils.c === */
char *_getenv(const char *name, char **envp);

/* === PROTOTYPES - main.c === */
void free_array(char **array);

int execute_builtin(char **args, char **envp, char **line);

#endif
