#include "shell.h"

/**
 * builtin_exit - Exits the shell
 * @args: Array of command arguments
 * @line: Input line to free before exiting
 *
 * Return: Does not return (calls exit)
 */
void builtin_exit(char **args, char **line)
{
	int exit_code = 0;

	/* Si exit a un argument, l'utiliser comme code de sortie */
	if (args[1])
	{
		exit_code = atoi(args[1]);
	}

	if (line && *line)
		free(*line);
	if (args)
		free(args);
	
	exit(exit_code);
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
 * Return: 1 if a builtin was executed, 0 otherwise
 */
int execute_builtin(char **args, char **envp, char **line)
{
	if (!args || !args[0])
		return (0);

	if (strcmp(args[0], "exit") == 0)
	{
		builtin_exit(args, line);
		return (1);
	}

	if (strcmp(args[0], "env") == 0)
	{
		builtin_env(envp);
		return (1);
	}

	return (0);
}
