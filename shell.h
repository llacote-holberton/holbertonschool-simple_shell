#ifndef SHELL_H
#define SHELL_H

/* === PROTOTYPES - main.c === */
int process_input(const char *received_input);
/* @note: put here at least while we develop to allow "unit tests" */

/* === PROTOTYPES - tokenizers.c === */
/* Public facing function to get array of substrings from a string */
char **tokenize_string(const char *string, char *delimiters);

#endif
