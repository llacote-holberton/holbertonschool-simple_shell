#include <stdlib.h> /* Required for malloc */
#include <string.h> /* Required for strtok */

/**
 * tokenize_string - Splits a string in array of components ("tokens").
 *   Goes from "I get string" to "Command ended".
 * @string: string to split in array of tokens.
 * @tokens: array to fill in with tokens.
 * Return: pointer to array of tokens or NULL if failure / no tokens.
 *
 * NOTES:
 * - I consider this function as a "reader" of input
 *   so I pass a constant pointer to stress it.
 * Initially returned (0 on success, >0 on fail) but required
 *   a triple pointer in prototype.
 *  => Better to create and return the array directly.
 */
char **tokenize_string(const char *string, char *delimiters)
{

	char *tokenized_string = NULL;         /* Needed because strtok alters it. */
	char **tokens = NULL; /* Array which will hold token(s) found if any. */
	size_t tokens_count = 0;          /* Computed with a "first pass".    */
	char *token = NULL;       /* Revolving variable for each token found. */
	size_t i = 0;   /* Iterator to fill up tokens array if we found some. */

	if (!delimiters)
		delimiters = " \n\t";           /* How to split. */
	tokenized_string = strdup(string);
	if (!tokenized_string)
	{
		/* @fixme me a call to "error_logger" or whatever name I give it */
		/* with just a shortcode like ALLOC_FAILED or something + context. */
		/* Ideally return would be the same code as long as >0. */
		return (NULL);
	}
	/* strtok is this weird beast which needs initializing outside loop. */
	token = strtok(tokenized_string, delimiters);
	if (token)
	{
		tokens_count = 1;
		while (token)
		{
			token = strtok(NULL, delimiters);
			tokens_count++;
		}
	}
	if (tokens_count == 0)
		return (NULL); /* @fixme Should we consider this a failure or valid case? */
	tokens = malloc(sizeof(char *) * tokens_count);
	if (!tokens)
	{
		free(tokenized_string); /* @fixme same note as above. */
		return (NULL);
	}
	/* Re-initializing the copy as the EOL would break the re-parsing. */
	free(tokenized_string);
	tokenized_string = strdup(string);
	if (!tokenized_string) /* @fixme same as above*/
		return (NULL);
	tokens[i] = strtok(tokenized_string, delimiters);
	for (i += 1; i < tokens_count; i++)
		tokens[i] = strtok(NULL, delimiters);

	return (tokens);
}
