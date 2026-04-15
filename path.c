#include "shell.h"
#include <string.h>

/**
 * get_path - Trouve le chemin d'une commande dans PATH
 * @command: La commande à chercher
 *
 * Return: Chemin complet ou NULL
 */
char *get_path(char *command)
{
	char *path_env, *path_copy, *dir, *full_path;
	struct stat st;
	int cmd_len, dir_len;

	/* Si c'est déjà un chemin absolu ou relatif */
	if (command[0] == '/' || command[0] == '.')
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		return (NULL);
	}

	/* Récupérer PATH */
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);

	/* Dupliquer PATH */
	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	/* Découper par ":" */
	dir = strtok(path_copy, ":");

	while (dir)
	{
		dir_len = strlen(dir);
		cmd_len = strlen(command);

		/* Allouer pour : dir + "/" + cmd + "\0" */
		full_path = malloc(dir_len + cmd_len + 2);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}

		/* Construire le chemin */
		strcpy(full_path, dir);
		strcat(full_path, "/");
		strcat(full_path, command);

		/* Vérifier si existe ET si exécutable */ 
		if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
	}
	
	free(path_copy);
	return (NULL);
}
