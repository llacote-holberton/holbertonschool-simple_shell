#include "shell.h"

/**
 * builtin_exit - Signals shell should exit
 * @last_cmd_exit_code: exit code of last non-built-in runned.
 *
 * Return: -1 to signal exit
 */
int builtin_exit(int last_cmd_exit_code)
{
	/* We want to propagate the previously runned exit */
	/* AND signal to "main" that we want to end the program. */
	/* THUS using a "negative encode/decode" dual-step process. */
	return (-1 - last_cmd_exit_code);
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
 * @last_cmd_exit_code: exit code of last non-built-in runned.
 *
 * Return: 1 if builtin executed, <0 if exit, 0 otherwise
 */
int execute_builtin(char **envp, char **args,	int last_cmd_exit_code)
{
	int result;

	if (!args || !args[0])
		return (0);

	if (strcmp(args[0], "exit") == 0)
	{
		result = builtin_exit(last_cmd_exit_code);
		return (result);
	}

	if (strcmp(args[0], "env") == 0)
	{
		builtin_env(envp);
		return (1);
	}

	return (0);
}
