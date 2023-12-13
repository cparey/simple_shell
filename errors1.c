#include "shell.h"

/**
 * safeAtoi - converts a string to an integer with error handling
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int safeAtoi(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;

	for (i = 0;  s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * printErrorInfo - prints an error message with additional customInfo
 * @customInfo: the parameter and return customInfo struct
 * @errorString: string containing specified error type
 * Return: 0 if no numbers in string, converted number otherwise
 *        -1 on error
 */
void printErrorInfo(customInfo_t *customInfo, char *errorString)
{
	printErrorString(customInfo->fname);
	printErrorString(": ");
	printInteger(customInfo->line_count, STDERR_FILENO);
	printErrorString(": ");
	printErrorString(customInfo->argv[0]);
	printErrorString(": ");
	printErrorString(errorString);
}

/**
 * printInteger - prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the file descriptor to write to
 *
 * Return: number of characters printed
 */
int printInteger(int input, int fd)
{
	int (*writeChar)(char) = write_character;
	int i, count = 0;
	unsigned int absolute, current;

	if (fd == STDERR_FILENO)
		writeChar = writeErrorCharacter;

	if (input < 0)
	{
		absolute = -input;
		writeChar('-');
		count++;
	}
	else
		absolute = input;

	current = absolute;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (absolute / i)
		{
			writeChar('0' + current / i);
			count++;
		}
		current %= i;
	}

	writeChar('0' + current);
	count++;

	return (count);
}

/**
 * convertNumberToString - converter function, a clone of itoa
 * @num: number
 * @base: base
 * @flags: argument flags
 *
 * Return: string
 */
char *convertNumberToString(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';
	}

	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;

	return (ptr);
}

/**
 * removeComments - function replaces first instance of '#' with '\0'
 * @buf: address of the string to modify
 *
 * Return: Always 0;
 */
void removeComments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}

