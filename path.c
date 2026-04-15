#include "shell.h"

/**
 * find_path_in_env - Trouve PATH dans environ
 * @envp: Variables d'environnement
 *
 * Return: Valeur de PATH ou NULL
 */
static char *find_path_in_env(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A' &&
		    envp[i][2] == 'T' && envp[i][3] == 'H' &&
		    envp[i][4] == '=')
		{
			return (envp[i] + 5);
		}
		i++;
	}
	return (NULL);
}

/**
 * check_direct_path - Vérifie si la commande est un chemin direct
 * @command: La commande à vérifier
 *
 * Return: Copie du chemin si valide, NULL sinon
 */
static char *check_direct_path(char *command)
{
	struct stat st;

	if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
	{
		if (stat(command, &st) == 0 &&
		    S_ISREG(st.st_mode) &&
		    access(command, X_OK) == 0)
			return (strdup(command));
	}
	return (NULL);
}

/**
 * build_full_path - Construit le chemin complet
 * @dir: Le dossier
 * @command: La commande
 *
 * Return: Chemin complet ou NULL
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
 * search_in_path - Cherche la commande dans PATH
 * @command: La commande à chercher
 * @path_copy: Copie du PATH
 *
 * Return: Chemin complet ou NULL
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
 * get_cmd_fullpath - Trouve le chemin d'une commande dans PATH
 * @command: La commande à chercher
 * @envp: Variables d'environnement
 *
 * Return: Chemin complet ou NULL
 */
char *get_cmd_fullpath(char *command, char **envp)
{
	char *path_env, *path_copy, *result;

	result = check_direct_path(command);
	if (result)
		return (result);

	path_env = find_path_in_env(envp);
	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	result = search_in_path(command, path_copy);
	free(path_copy);

	return (result);
}
