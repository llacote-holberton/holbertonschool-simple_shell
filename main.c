#include <stdio.h>  /* Required for printf */
#include <stdlib.h> /* Required for malloc */
#include <string.h> /* Required for strtok */
#include <unistd.h> /* Required for isatty */
#include "shell.h"  /* Custom functions    */

/*****************************************************************************/

/**
 * get_input_line - Returns the next segment of stdin (stops at 1st \n found).
 * @received_input: pointer to fill with line read from input.
 * @received_size: pointer to area holding number of characters read.
 *
 * Return: pointer to the retrieved string line.
 *
 * NOTES: confer ADR 003 and 004.
 * Responsability of freeing memory lies upon caller.
 */
static void get_input_line(char **received_input, size_t *received_size)
{
	int read_code;         /* Return code of getline, -1 = EndOfFile or error. */

	read_code = getline(received_input, received_size, stdin);
	if (read_code == -1)
	{
		free(*received_input);
		*received_input = NULL;
	}
	else if ((*received_input)[read_code - 1] == '\n')
	{
		(*received_input)[read_code - 1] = '\0';
	}
}


/**
 * process_input - Subprocessor.
 *   Goes from "I get string" to "Command ended".
 * @received_input: input retrieved in main with get_input_line.
 * @envp: execution environment (to "propagate down").
 * Return: 0 on success, error code on failure.
 * NOTES:
 * - I consider this function as a "reader" of input
 *   so I pass a constant pointer to stress it.
 * - As it delegates everything to subpointers it does not create
 *     duplicates of the input, just pass its pointer by value.
 */
int process_input(const char *received_input, char **envp)
{
	char **tokens = NULL; /* Placeholder for return of tokenize_string.  */
	char *command_fullpath = NULL; /* Placeholder command search result */
	char *tokenized_string = NULL; /* Needs to be here to free correctly. */
	int builtin_success;

	tokens = tokenize_string(received_input, NULL, &tokenized_string);

	if (tokens)
	{
		builtin_success = execute_builtin(tokens, envp, &tokenized_string);
		if (!builtin_success)
			command_fullpath = get_cmd_fullpath(tokens[0], envp);
	}
	if (command_fullpath)
	{
		execute_command(command_fullpath, tokens, envp);
		free(command_fullpath); /* IMU we don't need it anymore. */
	}
	/* Clean up everything */
	free(tokenized_string);
	free(tokens);

	return (0);
}

/**
 * main - Main entry point for simple shell.
 * @argc: count of provided arguments (shell itself counted)
 * @argv: array of strings representing line cut by spaces.
 * @envp: array of environment variables provided by caller.
 * Return: 0 in success, positive on error.
 *
 * NOTES: finally will be simpler than expected.
 * - No support for processing arguments given "beyond shell".
 *   User will need to either just wait for prompt, or run shell
 *   with input provided from "piping something into shell".
 * - No early check for ENV/PATH for now, will delegate.
 *     May change later once we have a working function to drill into env.
 * - IM = Interactive Mode. NIM = Non-Interactive Mode.
 */
int main(int argc, char **argv, char **envp)
{
	int is_interactive;
	char *received_input = NULL;
	size_t received_size = 0;
	const char *prompt = "$ ";

	is_interactive = isatty(STDIN_FILENO);

	while (1)
	{
		printf("%s", (is_interactive) ? "\n" : "");
		get_input_line(&received_input, &received_size);

		if (received_input == NULL)
		{
			printf("%s", (is_interactive) ? "\n" : "");
			break;
		}
		else
		{
			printf("%s", (is_interactive) ? "\n" : "");
			if (received_input[0] != '\0')
				process_input(received_input, envp);
		}
	}
	free(received_input);
	(void)argc;
	(void)argv;
	(void)envp;
	exit(0);
}


/*****************************************************************************/
