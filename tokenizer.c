#include "shell.h"

/**
 * split_string - splits a string into words. Repeat delimiters are ignored
 * @input_string: the input string
 * @delimiter_string: the delimiter string
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **split_string(char *input_string, char *delimiter_string)
{
	int i, j, k, m, num_words = 0;
	char **words;

	if (input_string == NULL || input_string[0] == '\0')
		return (NULL);
	if (!delimiter_string)
		delimiter_string = " ";
	for (i = 0; input_string[i] != '\0'; i++)
		if (!isDelimiter(input_string[i], delimiter_string) &&
			(isDelimiter(input_string[i + 1], delimiter_string) || !input_string[i + 1]))
			num_words++;

	if (num_words == 0)
		return (NULL);
	words = malloc((1 + num_words) * sizeof(char *));
	if (!words)
		return (NULL);
	for (i = 0, j = 0; j < num_words; j++)
	{
		while (isDelimiter(input_string[i], delimiter_string))
			i++;
		k = 0;
		while (!isDelimiter(input_string[i + k], delimiter_string) && input_string[i + k])
			k++;
		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j])
		{
			for (k = 0; k < j; k++)
				free(words[k]);
			free(words);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			words[j][m] = input_string[i++];
		words[j][m] = '\0';
	}
	words[j] = NULL;
	return (words);
}

/**
 * split_string_with_delimiter - splits a string into words
 * @input_string: the input string
 * @delimiter: the delimiter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **split_string_with_delimiter(char *input_string, char delimiter)
{
	int i, j, k, m, num_words = 0;
	char **words;

	if (input_string == NULL || input_string[0] == '\0')
		return (NULL);
	for (i = 0; input_string[i] != '\0'; i++)
		if ((input_string[i] != delimiter && input_string[i + 1] == delimiter) ||
			(input_string[i] != delimiter && !input_string[i + 1]) || input_string[i + 1] == delimiter)
			num_words++;
	if (num_words == 0)
		return (NULL);
	words = malloc((1 + num_words) * sizeof(char *));
	if (!words)
		return (NULL);
	for (i = 0, j = 0; j < num_words; j++)
	{
		while (input_string[i] == delimiter && input_string[i] != delimiter)
			i++;
		k = 0;
		while (input_string[i + k] != delimiter && input_string[i + k] && input_string[i + k] != delimiter)
			k++;
		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j])
		{
			for (k = 0; k < j; k++)
				free(words[k]);
			free(words);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			words[j][m] = input_string[i++];
		words[j][m] = '\0';
	}
	words[j] = NULL;
	return (words);
}

