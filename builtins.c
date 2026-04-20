#include "shell.h"

/**
 * is_valid_number - Checks if string is a valid number
 * @str: String to check
 *
 * Return: 1 if valid, 0 otherwise
 */
static int is_valid_number(char *str)
{
	int i = 0;

	if (!str || !str[0])
		return (0);

	/* Skip optional + or - */
	if (str[0] == '+' || str[0] == '-')
		i++;

	/* Must have at least one digit */
	if (!str[i])
		return (0);

	/* Check all remaining chars are digits */
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}

	return (1);
}

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
	{
		/* Check if argument is a valid number */
		if (!is_valid_number(args[1]))
		{
			/* Invalid argument - print error and return 2 */
			fprintf(stderr, "exit: Illegal number: %s\n", args[1]);
			return (-1 - 2);  /* Exit with code 2 */
		}

		exit_code = atoi(args[1]);
		
		/* Normalize to 0-255 range */
		exit_code = exit_code % 256;
		if (exit_code < 0)
			exit_code += 256;
	}

	return (-1 - exit_code);
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
