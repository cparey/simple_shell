#include "shell.h"

/**
 * checkInteractiveMode - returns true if shell is in interactive mode
 * @customInfo: pointer to the struct containing relevant information
 *
 * Return: 1 if in interactive mode and 0 if otherwise
 */
int checkInteractiveMode(customInfo_t *customInfo)
{
	return (isatty(STDIN_FILENO) && customInfo->input_fd <= 2);
}

/**
 * isDelimiter - checks if character is a delimiter
 * @ch: the character to check
 * @delimiter_string: the delimiter string
 * Return: 1 if true, 0 if false
 */
int isDelimiter(char ch, char *delimiter_string)
{
	while (*delimiter_string)
		if (*delimiter_string++ == ch)
			return (1);
	return (0);
}

/**
 * isAlphabetic - checks for alphabetic character
 * @ch: The character to check
 * Return: 1 if ch is alphabetic, 0 otherwise
 */
int isAlphabetic(int ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * stringToInteger - converts a string to an integer
 * @st: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 */
int stringToInteger(char *st)
{
	int i, signMultiplier = 1, digitFlag = 0, convertedNumber;
	unsigned int result = 0;

	for (i = 0;  st[i] != '\0' && digitFlag != 2; i++)
	{
		if (st[i] == '-')
			signMultiplier *= -1;

		if (st[i] >= '0' && st[i] <= '9')
		{
			digitFlag = 1;
			result *= 10;
			result += (st[i] - '0');
		}
		else if (digitFlag == 1)
			digitFlag = 2;
	}

	if (signMultiplier == -1)
		convertedNumber = -result;
	else
		convertedNumber = result;

	return (convertedNumber);
}

