#include "shell.h"

/**
 * _getenv - Récupère une variable d'environnement
 * @name: Nom de la variable (ex: "PATH")
 * @envp: Tableau des variables d'environnement
 *
 * Return: Valeur de la variable ou NULL
 */
char *_getenv(const char *name, char **envp)
{
	int index, counter;
	int name_len;

	if (!name || !envp)
		return (NULL);

	name_len = 0;
	while (name[name_len])
		name_len++;

	index = 0;
	while (envp[index])
	{
		/* Comparer le nom caractère par caractère */
		counter = 0;
		while (counter < name_len &&
		       envp[index][counter] == name[counter])
			counter++;

		/* Si match complet ET suivi de '=' */
		if (counter == name_len && envp[index][counter] == '=')
			return (envp[index] + counter + 1);

		index++;
	}

	return (NULL);
}
