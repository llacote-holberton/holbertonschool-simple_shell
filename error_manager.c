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
 * @function_name: name of function from which error was triggered.
 */
void log_internal_error(char *function_name)
{
	/* @important: "shell_name: line_number: function name: details"*/
	char *message_template = "%s: %d: %s: %s\n";
	char *shell_name = NULL;
	unsigned int line_number;
	char *error_details = NULL;

	shell_name = get_set_program_name(NULL);
	line_number = get_set_currentline_number(0);
	error_details = strerror(errno);

	fprintf
	(
		stderr,
		message_template,
		shell_name,
		line_number,
		function_name,
		error_details
	);
}

/**
 * log_functional_error - Wraps syscall error to give more info.
 * @command_tried: what was tried to be run as command, from input.
 */
void log_functional_error(char *command_tried)
{
	/* @important: "shell_name: line_number: command provided: details"*/
	char *message_template = "%s: %d: %s: %s\n";
	char *shell_name = NULL;
	unsigned int line_number;
	char *error_details = NULL;

	shell_name = get_set_program_name(NULL);
	line_number = get_set_currentline_number(0);
	error_details = strerror(errno);

	fprintf
	(
		stderr,
		message_template,
		shell_name,
		line_number,
		command_tried,
		error_details
	);
}

/**
	* log_error - Returns the template matching given code
	*   with the program name injected into it.
	* @error_code: arbitrary number must match one of the known cells.
	* @func_name: name of function in which error was triggered.
	* @cmd_tried: name of "command" which was attempted.
	*
	*/
void log_error(char *error_code, char *func_name, char *cmd_tried)
{
	/* Messages always start with shell name and line number */
	char *prefix_template = "%s: %d: ";
	static char msg_prefix[256];

	if (!func_name)
		func_name = "(null)";
	if (!cmd_tried)
		cmd_tried = "(null)";
	/* Preparing/Updating the "common part" of all error messages */
	sprintf
	(
		msg_prefix,
		prefix_template,
		get_set_program_name(NULL),
		get_set_currentline_number(0)
	);

	if (!error_code || error_code[0] == '\0')
		fprintf(stderr, " ");
	else if (strcmp(error_code, "INTERNAL_ERR") == 0)
		log_internal_error(func_name);
	else if (strcmp(error_code, "FUNCTIONAL_ERR") == 0)
		log_functional_error(cmd_tried);
	else if (strcmp(error_code, "NO_VALID_INPUT") == 0)
		fprintf(stderr, "provided input had nothing exploitable:\n");
	else if (strcmp(error_code, "CMD_NOT_FOUND") == 0)
		fprintf(stderr, "%s%s: not found\n", msg_prefix, cmd_tried);
	else if (strcmp(error_code, "NO_PATH") == 0)
		fprintf(stderr, "warning: PATH environment is empty/undefined.\n");
	else if (strcmp(error_code, "NO_ACCESS") == 0)
		fprintf(stderr, "Insufficient permissions to access/execute %s", cmd_tried);
}
