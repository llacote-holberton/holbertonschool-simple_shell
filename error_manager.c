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
 * get_error_template - Returns the template matching given code
 *   with the program name injected into it.
 * @code: arbitrary number must match one of the known cells.
 * Return: prepared template.
 */
static char *get_error_template(int code)
{
	/* @important: first %s is always program name. */
	/*             second %s is function name.      */
	static char *templates[20];
	static char *program_name;
	static char prepared_string[256];

	program_name = get_set_program_name(NULL);
	/* Potential solution for syscalls: universal template*/
	/* templates[0] = "%s: %s - %s" */
	/*  program name, function name, error msg*/
	templates[1] = "%s: Malloc failed in %%s";
	templates[2] = "%s: Strdup failed in %%s";
	templates[3] = "%s: Getline read was interrupted:";
	templates[4] = "%s: Fork, child creation failed";
	templates[5] = "%s: Execve, process interrupted";
	templates[6] = "%s: Failure when attempting to printf/putchar";
	templates[7] = "%s: Command %%s cannot be found";
	templates[8] = "%s: Permission to run %%s denied for user %%s";
	templates[9] = "%s: No PATH found!";
	templates[10] = "%s: nothing found for path %%s";
	templates[11] = "%s: Builtin %s unknown";
	/* @important: TO BE REWORKED (or removed) */
	/* => Syscalls errors must be exactly the same as with Bash. */

	if (templates[code])
		sprintf(prepared_string, templates[code], program_name);
	else
		prepared_string = NULL;

	return (prepared_string);
}


/**
 * send_error_message - Prints formatted message on standard error.
 * @code: number indicating specific error.
 * @function_name: function from which error was triggered.
 */
void send_error_message(unsigned int code, char *function_name)
{
	char *error_template = NULL;
	/*char *error_message = NULL;*/
	FILE *stream = stderr;

	if (!function_name)
		function_name = "Unknown func";
	error_template = get_error_template(code);
	if (!error_template)
	{
		error_template = "Nothing found for code %d called from %s\n";
		fprintf(stderr, error_template, code, function_name);
	}
	fprintf(stream, error_template, function_name);
}
