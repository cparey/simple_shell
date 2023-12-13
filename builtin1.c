#include "shell.h"

/**
 * displayHistory - displays the history list, one command by line,
 *                  preceded with line numbers, starting at 0.
 * @customInfo: Structure containing potential arguments.
 * Return: Always 0
 */
int displayHistory(customInfo_t *customInfo)
{
	printList(customInfo->history);
	return (0);
}

/**
 * unsetAlias - unsets alias to string
 * @customInfo: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unsetAlias(customInfo_t *customInfo, char *str)
{
	char *p, ch;
	int ret;

	p = findCharacterInString(str, '=');
	if (!p)
		return (1);
	ch = *p;
	*p = 0;
	ret = deleteNodeAtIndex(&(customInfo->alias),
		getNodeIndex(customInfo->alias, nodeStartsWith(customInfo->alias, str, -1)));
	*p = ch;
	return (ret);
}

/**
 * setAlias - sets alias to string
 * @customInfo: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int setAlias(customInfo_t *customInfo, char *str)
{
	char *p;

	p = findCharacterInString(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unsetAlias(customInfo, str));

	unsetAlias(customInfo, str);
	return (addNodeEnd(&(customInfo->alias), str, 0) == NULL);
}

/**
 * printAlias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int printAlias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = findCharacterInString(node->str, '=');
		for (a = node->str; a <= p; a++)
			write_character(*a);
		write_character('\'');
		print_string(p + 1);
		print_string("'\n");
		return (0);
	}
	return (1);
}

/**
 * manageAlias - mimics the alias builtin (man alias)
 * @customInfo: Structure containing potential arguments.
 * Return: Always 0
 */
int manageAlias(customInfo_t *customInfo)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (customInfo->argc == 1)
	{
		node = customInfo->alias;
		while (node)
		{
			printAlias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; customInfo->argv[i]; i++)
	{
		p = findCharacterInString(customInfo->argv[i], '=');
		if (p)
			setAlias(customInfo, customInfo->argv[i]);
		else
			printAlias(nodeStartsWith(customInfo->alias, customInfo->argv[i], '='));
	}

	return (0);
}

