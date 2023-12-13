#include "shell.h"

/**
 * copyStringWithLimit - copies a string with a character limit
 * @destination: the destination string to be copied to
 * @source: the source string
 * @limit: the maximum number of characters to be copied
 * Return: the copied string
 */
char *copyStringWithLimit(char *destination, char *source, int limit)
{
	int i, j;
	char *result = destination;

	i = 0;
	while (source[i] != '\0' && i < limit - 1)
	{
		destination[i] = source[i];
		i++;
	}
	if (i < limit)
	{
		j = i;
		while (j < limit)
		{
			destination[j] = '\0';
			j++;
		}
	}
	return result;
}

/**
 * concatenateStringWithLimit - concatenates two strings with a character limit
 * @destination: the first string
 * @source: the second string
 * @limit: the maximum number of bytes to be used
 * Return: the concatenated string
 */
char *concatenateStringWithLimit(char *destination, char *source, int limit)
{
	int i, j;
	char *result = destination;

	i = 0;
	j = 0;
	while (destination[i] != '\0')
		i++;
	while (source[j] != '\0' && j < limit)
	{
		destination[i] = source[j];
		i++;
		j++;
	}
	if (j < limit)
		destination[i] = '\0';

	return result;
}

/**
 * findCharacterInString - locates a character in a string
 * @s: the string to be parsed
 * @c: the character to look for
 * Return: (s) a pointer to the memory area s
 */
char *findCharacterInString(char *s, char c)
{
	do {
		if (*s == c)
			return s;
	} while (*s++ != '\0');

	return NULL;
}

