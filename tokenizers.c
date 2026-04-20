#include <stdlib.h> /* Required for malloc */
#include <string.h> /* Required for strtok */
#include "shell.h"  /* Required for error push */

/**
 * get_default_delimiters - "Helper" function just holding
 *   the default delimiters for when none are provided.
 * Return: pointer to a string aggregating delimiters.
 *
 * NOTE: added just because same need in both count_tokens
 *   and tokenize_string.
 */
static const char *get_default_delimiters(void)
{
	return (" \n\t");
}

/**
 * count_tokens - "Helper" function which just counts in how many
 *   "parts" a string would be split into with provided delimiter set.
 *   Goes from "I get string" to "Command ended".
 * @string: string to split in array of tokens.
 * @delimiters: set of characters to consider delimiters.
 * Return: pointer to array of tokens or NULL if failure / no tokens.
 *
 * NOTE: no check for NULL string because helper function.
 *   => NOT called if string to check doesn't exist in the first place.
 */
static size_t count_tokens(const char *string, const char *delimiters)
{
	char *copy_for_count = NULL;
	size_t tokens_count = 0;
	char *chunk = NULL;

	if (!delimiters)
		delimiters = get_default_delimiters();

	copy_for_count = strdup(string);
	if (copy_for_count)
	{
		chunk = strtok(copy_for_count, delimiters);
		while (chunk)
		{
			tokens_count++;
			chunk = strtok(NULL, delimiters);
		}
		free(copy_for_count);
	}
	else
		/*log_error("INTERNAL_ERR", "count_tokens", NULL);*/

	return (tokens_count);
}

/**
 * tokenize_string - Splits a string in array of components ("tokens").
 *   Goes from "I get string" to "Command ended".
 * @string: string to split in array of tokens.
 * @delimiters: set of characters to consider delimiters.
 * @tokenized_string: pointer to area receiving the processed string.
 * Return: pointer to array of tokens or NULL if failure / no tokens.
 *
 * NOTES:
 * - I consider this function as a "reader" of input
 *   so I pass a constant pointer to stress it.
 * Initially returned (0 on success, >0 on fail) but required
 *   a triple pointer in prototype.
 *  => Better to create and return the array directly.
 */
char **tokenize_string(const char *string, const char *delimiters,
											 char **tokenized_string)
{
	/* Third argument to ensure correct free once finished. */
	char **tokens = NULL; /* Array which will hold token(s) found if any. */
	size_t tokens_count = 0;          /* Computed with a "first pass".    */
	size_t i = 0;   /* Iterator to fill up tokens array if we found some. */

	if (!delimiters)
		delimiters = get_default_delimiters();

	tokens_count = count_tokens(string, delimiters);
	if (tokens_count == 0)
		return (NULL);

	/* @warning DO NOT FORGET the "space" for the mandatory NULL cell! */
	tokens = malloc(sizeof(char *) * (tokens_count + 1));
	if (!tokens)
	{
		/*log_error("INTERNAL_ERR", "tokenize_string", NULL);*/
		return (NULL);
	}

	*tokenized_string = strdup(string);
	if (!tokenized_string) /* @fixme same as above*/
	{
		/*log_error("INTERNAL_ERR", "tokenize_string", NULL);*/
		free(tokens);
		return (NULL);
	}

	tokens[i] = strtok(*tokenized_string, delimiters);
	for (i += 1; i < tokens_count; i++)
		tokens[i] = strtok(NULL, delimiters);
	/* @warning DO NOT forget the mandatory NULL cell for others to know end. */
	tokens[i] = NULL;
	/* @warning CANNOT FREE TOKENS, IT WILL BE RESPONSABILITY OF CALLER */
	return (tokens);
}
