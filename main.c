#include <stdio.h>  /* Required for printf */
#include <stdlib.h> /* Required for malloc */
#include <string.h> /* Required for strtok */
#include <unistd.h> /* Required for isatty */
#include "shell.h"  /* Custom functions    */

/*****************************************************************************/

/**
 * get_input_line - Returns the next segment of stdin (stops at 1st \n found).
 * Return: pointer to the retrieved string line.
 *
 * NOTES: confer ADR 003 and 004.
 * Responsability of freeing memory lies upon caller.
 */
static void get_input_line(char **received_input, size_t *received_size)
{
	/* @note allocation is automatically done by getline. */
	/* "read" was confusing: infinitive/past?     */
	int read_code;         /* Return code of getline, -1 = EndOfFile or error. */

	/* Pass addresses so getline can modify. stdin is provided by compiler.    */
	read_code = getline(received_input, received_size, stdin);
	if (read_code == -1)
	{
		free(*received_input);
		*received_input = NULL;
	}
	else
	{
		/* Removing endline by replacing with EOL to "clean line" */
		/* for parsing. -1 to account for the EOL char */
		(*received_input)[read_code - 1] = '\0';
	}
}


/**
 * process_input - Subprocessor.
 *   Goes from "I get string" to "Command ended".
 * @received_input: input retrieved in main with get_input_line.
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

	/* Try and get an array of tokens. */
	tokens = tokenize_string(received_input, NULL);
	/* @fixme implement case "tokens empty or NULL" */

	/* @TEMPORARY */
	printf("\nFirst token is %s\n", (tokens) ? tokens[0] : "EMPTY ARRAY");

	/* IF TOKENS NON NULL get PATH and send to SEARCH */
	if (tokens)
		command_fullpath = get_cmd_fullpath(tokens[0], envp);
	printf("Command found: %s\n", command_fullpath);
	/* IF COMMAND FOUND EXECUTE */
	if (command_fullpath)
		execute_command(command_fullpath, tokens, envp);
	/* Clean up everything */
	free(tokens);

	/* @temporary */
	(void)tokens;

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
	/* First things first: declaring the variables needed at that level.        */
	int is_interactive;      /* Used to know whether to display prompt.         */
	char *received_input = NULL; /* Line retrieved from reading STDIN_FILENO.   */
	size_t received_size = 0;                /* Used by getline for allocation. */
	const char *prompt = "$ "; /* Line start dispayed to user in IM. */

	/* Will return 1 if shell runned without a stream provided yet (no piping). */
	is_interactive = isatty(STDIN_FILENO);

	/* Start the input management loop, same process for both IM and NIM. */
	while (1)
	{
		if (is_interactive)
			printf("%s", prompt);
		get_input_line(&received_input, &received_size);
		if (received_input == NULL)
		{
			/* Things to free? */
			/* Error Manager to call? How to know whether -1 means EOF or error?    */
			putchar('\n');
			break;
		}
		else
		{
			/* Display newline immediately BEFORE any potential command output. */
			putchar('\n');
			/* Magic happens. Try and parse line as tokens. Try and find command. */
			/* Try and execute command. Handle errors of all kinds. */
			printf("Hey hey! I received the input properly!! Here it is! \n");
			printf("%s", received_input);
			/* Guard clause to avoid useless calls */
			if (received_input[0] != '\0')
				process_input(received_input, envp);
			/* Finally free everything main "owns". */
			/* Free received_input? Or only after loop ended? */
		}
	}
	/* Clean up */
	free(received_input);
	/* @temporary */
	(void)argc;
	(void)argv;
	(void)envp;
	exit(0);
}


/*****************************************************************************/
