#include "shell.h"

/**
 * getEnvironmentStrings - returns the string array copy of our environment
 * @customInfo: Structure containing potential arguments.
 * Return: Array of strings representing the environment.
 */
char **getEnvironmentStrings(customInfo_t *customInfo)
{
	if (!customInfo->environ || customInfo->env_changed)
	{
		customInfo->environ = listToStrings(customInfo->env);
		customInfo->env_changed = 0;
	}

	return customInfo->environ;
}

/**
 * remEnvironmentVariable - Remove an environment variable
 * @customInfo: Structure containing potential arguments.
 * @var: the string representing the environment variable property
 * Return: 1 on delete, 0 otherwise
 */
int remEnvironmentVariable(customInfo_t *customInfo, char *var)
{
	list_t *node = customInfo->env;
	size_t index = 0;
	char *p;

	if (!node || !var)
		return 0;

	while (node)
	{
		p = string_starts_with(node->str, var);
		if (p && *p == '=')
		{
			customInfo->env_changed = deleteNodeAtIndex(&(customInfo->env), index);
			index = 0;
			node = customInfo->env;
			continue;
		}
		node = node->next;
		index++;
	}
	return customInfo->env_changed;
}

/**
 * setEnvironmentVariable - Initialize a new environment variable,
 *             or modify an existing one
 * @customInfo: Structure containing potential arguments.
 * @var: the string representing the environment variable property
 * @value: the string representing the environment variable value
 * Return: 0 on success, 1 on failure
 */
int setEnvironmentVariable(customInfo_t *customInfo, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return 1;

	buf = malloc(string_length(var) + string_length(value) + 2);
	if (!buf)
		return 1;
	copy_string(buf, var);
	string_concatenate(buf, "=");
	string_concatenate(buf, value);
	node = customInfo->env;
	while (node)
	{
		p = string_starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			customInfo->env_changed = 1;
			return 0;
		}
		node = node->next;
	}
	addNodeEnd(&(customInfo->env), buf, 0);
	free(buf);
	customInfo->env_changed = 1;
	return 0;
}

