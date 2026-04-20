#include "shell.h"

/**
 * builtin_exit - Signals shell should exit
 * @args: Array of command arguments
 * @line: Input line (unused here)
 *
 * Return: Exit code (negative means exit requested)
 */
int builtin_exit(char **args, char **line)
{
	int exit_code = 0;

	(void)line;

	if (args[1])
		exit_code = atoi(args[1]);

	return (-1 - exit_code);  /* -1 = exit(0), -2 = exit(1), etc. */
}

/**
 * builtin_env - Displays all environment variables
 * @envp: Array of environment variables
 */
void builtin_env(char **envp)
{
	int index;

	if (!envp)
		return;

	index = 0;
	while (envp[index])
	{
		printf("%s\n", envp[index]);
		index++;
	}
}

/**
 * execute_builtin - Executes a builtin if recognized
 * @args: Array of command arguments
 * @envp: Environment variables
 * @line: Complete line to free if exit is called
 *
 * Return: 1 if builtin executed, <0 if exit, 0 otherwise
 */
int execute_builtin(char **args, char **envp, char **line)
{
	int result;

	if (!args || !args[0])
		return (0);

	if (strcmp(args[0], "exit") == 0)
	{
		result = builtin_exit(args, line);
		return (result);
	}

	if (strcmp(args[0], "env") == 0)
	{
		builtin_env(envp);
		return (1);
	}

	return (0);
}
