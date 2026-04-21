#include "shell.h"

/**
 * check_direct_path - Checks if command is a direct path
 * @command: Command to check
 *
 * Return: Copy of path if valid, NULL otherwise
 */
static char *check_direct_path(char *command)
{
	struct stat st;

	/* @note: useless BECAUSE a) internal func + guard in "public" caller. */
	/* if (!command || command[0] == '\0') return (NULL); */

	/* @note: adding && command[1] == '/' makes code miss ../ or hidden files. */
	if (command[0] == '/' || command[0] == '.')
	{
		if (stat(command, &st) == 0 &&
		    S_ISREG(st.st_mode) &&
		    access(command, X_OK) == 0)
			return (strdup(command));
	}
	return (NULL);
}

/**
 * build_full_path - Builds the full path
 * @dir: Directory
 * @command: Command
 *
 * Return: Full path or NULL
 */
static char *build_full_path(char *dir, char *command)
{
	char *full_path;
	int dir_len, cmd_len;

	dir_len = strlen(dir);
	cmd_len = strlen(command);

	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);

	strcpy(full_path, dir);
	strcat(full_path, "/");
	strcat(full_path, command);

	return (full_path);
}

/**
 * search_in_path - Searches for command in PATH
 * @command: Command to search
 * @path_copy: Copy of PATH
 *
 * Return: Full path or NULL
 */
static char *search_in_path(char *command, char *path_copy)
{
	char *dir, *full_path;
	struct stat st;

	dir = strtok(path_copy, ":");

	while (dir)
	{
		full_path = build_full_path(dir, command);
		if (!full_path)
			return (NULL);

		if (stat(full_path, &st) == 0 &&
		    S_ISREG(st.st_mode) &&
		    access(full_path, X_OK) == 0)
			return (full_path);

		free(full_path);
		dir = strtok(NULL, ":");
	}

	return (NULL);
}

/**
 * get_cmd_fullpath - Finds command path in PATH
 * @command: Command to search
 * @envp: Environment variables
 *
 * Return: Full path or NULL
 */
char *get_cmd_fullpath(char *command, char **envp)
{
	char *path_env, *path_copy, *result;

	if (!command || command[0] == '\0')
		return (NULL);

	result = check_direct_path(command);
	if (result)
		return (result);

	path_env = _getenv("PATH", envp);
	if (!path_env || path_env[0] == '\0')
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	result = search_in_path(command, path_copy);
	free(path_copy);

	return (result);
}
