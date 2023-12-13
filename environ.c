#include "shell.h"

/**
 * displayEnvironment - prints the current environment
 * @customInfo: Structure containing potential arguments.
 * Return: Always 0
 */
int displayEnvironment(customInfo_t *customInfo)
{
	printListStr(customInfo->env);
	return (0);
}

/**
 * getEnvironmentVariable - gets the value of an environment variable
 * @customInfo: Structure containing potential arguments.
 * @name: environment variable name
 *
 * Return: the value of the environment variable
 */
char *getEnvironmentVariable(customInfo_t *customInfo, const char *name)
{
	list_t *node = customInfo->env;
	char *p;

	while (node)
	{
		p = string_starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * initializeEnvironmentVariable - Initialize a new environment variable,
 *             or modify an existing one
 * @customInfo: Structure containing potential arguments.
 * Return: Always 0
 */
int initializeEnvironmentVariable(customInfo_t *customInfo)
{
	if (customInfo->argc != 3)
	{
		printErrorString("Incorrect number of arguments\n");
		return (1);
	}
	if (setEnvironmentVariable(customInfo, customInfo->argv[1], customInfo->argv[2]))
		return (0);
	return (1);
}

/**
 * unsetEnvironmentVariable - Remove an environment variable
 * @customInfo: Structure containing potential arguments.
 * Return: Always 0
 */
int unsetEnvironmentVariable(customInfo_t *customInfo)
{
	int i;

	if (customInfo->argc == 1)
	{
		printErrorString("Too few arguments.\n");
		return (1);
	}
	for (i = 1; i <= customInfo->argc; i++)
		remEnvironmentVariable(customInfo, customInfo->argv[i]);

	return (0);
}

/**
 * populateEnvironmentList - populates environment linked list
 * @customInfo: Structure containing potential arguments.
 * Return: Always 0
 */
int populateEnvironmentList(customInfo_t *customInfo)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		addNodeEnd(&node, environ[i], 0);
	customInfo->env = node;
	return (0);
}

