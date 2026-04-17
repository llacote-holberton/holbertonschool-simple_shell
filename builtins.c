#include "shell.h"

/**
 * builtin_exit - Quitte le shell proprement
 * @args: Tableau d'arguments de la commande
 * @line: Ligne d'entree a liberer avant de quitter
 *
 * Description: Libere la memoire allouee puis termine le processus
 * Return: Ne retourne pas (appelle exit)
 */
void builtin_exit(char **args, char **line)
{
	if (line && *line)
		free(*line);
	if (args)
		free(args);
	exit(0);
}
/**
 * builtin_env - Displays all environment variables
 * @envp: Array of environment variables
 *
 * Description: Iterates through and prints each environment variable
 * Return: Nothing
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
 * execute_builtin - Execute un builtin s'il est reconnu
 * @args: Tableau d'arguments de la commande
 * @envp: Variables d'environnement
 * @line: Ligne complete a liberer si exit est appele
 *
 * Description: Identifie et execute le builtin correspondant
 * Return: 1 si un builtin a ete execute, 0 sinon
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
