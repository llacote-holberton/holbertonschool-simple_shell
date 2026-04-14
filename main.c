#include <stdio.h>  /* Required for printf */
#include <stdlib.h> /* Required for malloc */
#include <string.h> /* Required for strtok */
#include <unistd.h> /* Required for isatty */

/*****************************************************************************/

/**
 * get_input_line - Returns the next segment of stdin (stops at 1st \n found).
 * Return: pointer to the retrieved string line.
 *
 * NOTES: confer ADR 003 and 004.
 */
char *get_input_line(void)
{
	/* @note allocation is automatically done by getline. */
	char *received_input = NULL; /* "read" was confusing: infinitive/past?     */
	size_t received_size;                   /* Used by getline for allocation. */
	int read_code;         /* Return code of getline, -1 = EndOfFile or error. */

	/* Pass addresses so getline can modify. stdin is provided by compiler.    */
	read_code = getline(&received_input, &received_size, stdin);
	if (read_code == -1)
	{
		free(received_input);
		received_input = NULL;
	}
	return (received_input);
	/* @fixme how/when to free memory of what was read??? */
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
	char *received_input;        /* Line retrieved from reading STDIN_FILENO.   */
	const char *prompt = "$ "; /* Line start dispayed to user in IM. */

	/* Will return 1 if shell runned without a stream provided yet (no piping). */
	is_interactive = isatty(STDIN_FILENO);

	/* Start the input management loop, same process for both IM and NIM. */
	while (1)
	{
		if (is_interactive)
			printf("%s", prompt);
		received_input = get_input_line();
		if (received_input == NULL)
		{
			/* Things to free? */
			/* Error Manager to call? How to know whether -1 means EOF or error?    */
			break;
		}
		else
		{
			/* Magic happens. Try and parse line as tokens. Try and find command. */
			/* Try and execute command. Handle errors of all kinds. */
			printf("Hey hey! I received the input properly!! Here it is! \n");
			printf("%s", received_input);
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
