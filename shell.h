#ifndef SHELL_H
#define SHELL_H

/* === PROTOTYPES - tokenizers.c === */
/* Public facing function to get array of substrings from a string */
char **tokenize_string(const char *string, char *delimiters);

#endif
