#include <stdio.h>  /* Required for printf */
#include <stdlib.h> /* Required for malloc */
#include "shell.h"  /* Custom functions    */


/* @NOTE: yes, we know it's technically not a great word for what we meant. */
/*   but executor is so overrated. */

/**
 * execute_command - Responsible for handling the run
 *   of a given command with arguments inside
 *   a spawned child process and handling any errors.
 * @command: full path of the command to run
 * @arguments: array of arguments altering/configuring command behaviour.
 *   Note that the first argument is the command name/path "as provided".
 * @envp: execution environment to propagate to the child process.
 * Return: 0 if success, positive if error.
 *
 * NOTE: defining codes following convention used by many shells. Confer
 * https://www.networkworld.com/
 *   article/3546937/understanding-exit-codes-on-linux-2.html
 */
int execute_command(const char *command, char **arguments, char **envp)
{
	pid_t child_pid;
	int status;      /* Filled by wait with "binary-coded info" */
	int child_exit_code = 0;

	child_pid = fork();
	if (child_pid == -1)
	{
		return (-1); /* HOW TO MANAGE ERRORS??? Read errno */
	}
	if (child_pid == 0)
	{
		execve(command, arguments, envp);
		exit(EXIT_FAILURE);
	}

	/* Status gets info we can check with special functions. */
	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
	{
		/* @note: two-step is mandatory because program can be interrupted. */
		child_exit_code = WIFEXITED(status);
	}
	else if (WIFSIGNALED(status))
	{
		/* Convention, confer articles on exit codes from networkworld */
		child_exit_code = (128 + WIFSIGNALED(status));
	}
	return (child_exit_code);
}
