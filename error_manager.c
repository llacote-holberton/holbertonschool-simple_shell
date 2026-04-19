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

/**
 * get_set_currentline_number - Returns the number of lines
 *   read so far (including current).
 * @increment: 0 means "read count", >0 means "increment by 1"
 * Return: current number of lines read.
 *
 * NOTE: first name was get_set_inputlines_count but IMO
 *   the new one is more adequate with intended use.
 */
unsigned int get_set_currentline_number(int increment)
{
	/* Static -> automatically initialized to 0. */
	static unsigned int count;

	if (increment)
		count++;
	return (count);
}

/**
 * log_internal_error - Internal debugger (optional).
 * @func_name: name of function from which error was triggered.
 */
void log_internal_error(char *func_name)
{
	/* @important: "shell_name: line_number: command provided"*/
	char *prefix_tmpl = "%s: %d: %s";
	static char msg_prefix[256];
	char *name = NULL;
	unsigned int line;

	name = get_set_program_name(NULL);
	line = get_set_currentline_number(0);
	sprintf(msg_prefix, prefix_tmpl, name, line, func_name);

	fprintf(stderr, "%s: %s\n", msg_prefix, strerror(errno));
}

/**
 * log_functional_error - Wraps syscall error to give more info.
 * @cmd_tried: what was tried to be run as command, from input.
 */
void log_functional_error(char *cmd_tried)
{
	/* @important: "shell_name: line_number: command provided"*/
	char *prefix_tmpl = "%s: %d: %s";
	static char msg_prefix[256];
	char *name = NULL;
	int line_number;

	/* Preparing the "common part of all error messages". */
	name = get_set_program_name(NULL);
	line_number = get_set_currentline_number(0);
	sprintf(msg_prefix, prefix_tmpl, name, line_number, cmd_tried);
	/* Printing the complete error message on STDERR_NO*/
	fprintf(stderr, "%s: %s\n", msg_prefix, strerror(errno));
}
