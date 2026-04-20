#include "shell.h"

/**
 * get_input_line - Reads a line from stdin
 * @received_input: Pointer to buffer
 * @received_size: Size of buffer
 */
static void get_input_line(char **received_input, size_t *received_size)
{
	ssize_t read_code;

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
 * process_input - Processes a command
 * @received_input: Command string
 * @envp: Environment variables
 * @shell_name: Shell name
 * @line_number: Line number
 *
 * Return: Exit code
 */
int process_input(const char *received_input, char **envp,
                 char *shell_name, int line_number)
{
	char **tokens = NULL;
	char *command_fullpath = NULL;
	char *tokenized_string = NULL;
	int builtin_success;
	int command_exit_code = 0;

	tokens = tokenize_string(received_input, NULL, &tokenized_string);

	if (!tokens)
		return (0);

	builtin_success = execute_builtin(tokens, envp, &tokenized_string);
	
	if (builtin_success)
	{
		if (tokenized_string)
			free(tokenized_string);
		if (tokens)
			free(tokens);
		return (0);
	}

	command_fullpath = get_cmd_fullpath(tokens[0], envp);

	if (command_fullpath)
	{
		command_exit_code = execute_command(command_fullpath, tokens, envp);
		free(command_fullpath);
	}
	else
	{
		command_exit_code = 127;
		fprintf(stderr, "%s: %d: %s: not found\n",
			shell_name, line_number, tokens[0]);
	}

	if (tokenized_string)
		free(tokenized_string);
	if (tokens)
		free(tokens);

	return (command_exit_code);
}

/**
 * main - Entry point
 * @argc: Argument count
 * @argv: Argument vector
 * @envp: Environment
 *
 * Return: Last command exit code
 */
int main(int argc, char **argv, char **envp)
{
	int is_interactive;
	char *received_input = NULL;
	size_t received_size = 0;
	const char *prompt = "($) ";
	int line_number = 0;
	int process_return = 0;

	(void)argc;
	is_interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (is_interactive)
			printf("%s", prompt);

		get_input_line(&received_input, &received_size);

		if (received_input == NULL)
		{
			if (is_interactive)
				putchar('\n');
			break;
		}

		if (received_input[0] != '\0')
		{
			line_number++;
			process_return = process_input(received_input, envp, argv[0], line_number);
		}
	}

	free(received_input);
	exit(process_return);
}
