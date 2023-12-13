#include "shell.h"

/**
 * is_chain_delimiter - test if the current character in the buffer is a chain delimiter
 * @customInfo: the parameter struct
 * @buf: the character buffer
 * @p: address of the current position in buf
 *
 * Return: 1 if chain delimiter, 0 otherwise
 */
int is_chain_delimiter(customInfo_t *customInfo, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		customInfo->cmd_buf_type = CHAIN_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		customInfo->cmd_buf_type = CHAIN_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		customInfo->cmd_buf_type = CHAIN_CUSTOM;
	}
	else
		return 0;
	*p = j;
	return 1;
}

/**
 * check_chain_continuation - checks if we should continue chaining based on the last status
 * @customInfo: the parameter struct
 * @buf: the character buffer
 * @p: address of the current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void check_chain_continuation(customInfo_t *customInfo, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (customInfo->cmd_buf_type == CHAIN_AND)
	{
		if (customInfo->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (customInfo->cmd_buf_type == CHAIN_OR)
	{
		if (!customInfo->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * replace_aliases - replaces aliases in the tokenized string
 * @customInfo: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_aliases(customInfo_t *customInfo)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = nodeStartsWith(customInfo->alias, customInfo->argv[0], '=');
		if (!node)
			return 0;
		free(customInfo->argv[0]);
		p = findCharacterInString(node->str, '=');
		if (!p)
			return 0;
		p = duplicate_string(p + 1);
		if (!p)
			return 0;
		customInfo->argv[0] = p;
	}
	return 1;
}

/**
 * replace_variables - replaces variables in the tokenized string
 * @customInfo: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_variables(customInfo_t *customInfo)
{
	int i = 0;
	list_t *node;

	for (i = 0; customInfo->argv[i]; i++)
	{
		if (customInfo->argv[i][0] != '$' || !customInfo->argv[i][1])
			continue;

		if (!string_compare(customInfo->argv[i], "$?"))
		{
			replace_string(&(customInfo->argv[i]),
				duplicate_string(convertNumberToString(customInfo->status, 10, 0)));
			continue;
		}
		if (!string_compare(customInfo->argv[i], "$$"))
		{
			replace_string(&(customInfo->argv[i]),
				duplicate_string(convertNumberToString(getpid(), 10, 0)));
			continue;
		}
		node = nodeStartsWith(customInfo->env, &customInfo->argv[i][1], '=');
		if (node)
		{
			replace_string(&(customInfo->argv[i]),
				duplicate_string(findCharacterInString(node->str, '=') + 1));
			continue;
		}
		replace_string(&customInfo->argv[i], duplicate_string(""));
	}
	return 0;
}

/**
 * replace_string - replaces a string
 * @old: address of the old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return 1;
}

