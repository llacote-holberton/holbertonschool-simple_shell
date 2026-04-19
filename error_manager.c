/* === FILE HOLDING FUNCTIONS RELATED TO ERROR PROPAGATION */
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

/* ALL WILL NEED TO BE DECLARED IN CUSTOM HEADER */

/**
 * get_set_program_name - Gets/Sets name used for error messages.
 * @string: program name to store.
 * Return: stored program name.
 *
 * NOTE: "dual-mode" function: call with non-null string
 *   to define/update the program name.
 *   Call with NULL string to get whatever was stored.
 * Normally only main() calls with argument, everyone else
 *   calls with NULL to get the string.
 */
char *get_set_program_name(char *string)
{
	static char *program_name;

	if (!program_name && string && strlen(string) > 1)
		program_name = string;
	return ((program_name) ? program_name : "SHELL! ");
}

