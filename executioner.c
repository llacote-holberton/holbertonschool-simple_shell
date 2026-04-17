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
 */
int execute_command(const char *command, char **arguments, char **envp)
{
	pid_t child_pid;
	int status;      /* Filled by wait with "binary-coded info" */

	/* Create a child process */
	child_pid = fork();
	if (child_pid == -1)
		return (-1); /* HOW TO MANAGE ERRORS??? Read errno */
	/* Execute command within child */
	if (child_pid == 0)
	{
		execve(command, arguments, envp);
		/* @warning do not forget to terminate child process IF execve */
		/* didn't finish properly (in which case it exits automatically) */
		exit(EXIT_FAILURE);
	}

	wait(&status); /* Status gets info we can check with special functions. */
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		printf("Command failed with code: %d\n", WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("Killed by signal: %d\n", WTERMSIG(status));
	return (0);
}
