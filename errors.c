#include "shell.h"

/**
 * printErrorString - prints an error string to stderr
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void printErrorString(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		writeErrorCharacter(str[i]);
		i++;
	}
}

/**
 * writeErrorCharacter - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int writeErrorCharacter(char c)
{
	static int i;
	static char buf[WRITE_BUFFER_SIZE];

	if (c == BUFFER_FLUSH || i >= WRITE_BUFFER_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BUFFER_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * writeCharacterToFD - writes the character c to given file descriptor
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int writeCharacterToFD(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUFFER_SIZE];

	if (c == BUFFER_FLUSH || i >= WRITE_BUFFER_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BUFFER_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * printStringToFD - prints an input string to a given file descriptor
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of characters put
 */
int printStringToFD(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += writeCharacterToFD(*str++, fd);
	}
	return (i);
}

