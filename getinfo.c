#include "shell.h"

/**
 * initializeInfo - initializes customInfo_t struct
 * @customInfo: struct address
 */
void initializeInfo(customInfo_t *customInfo)
{
	customInfo->arg = NULL;
	customInfo->argv = NULL;
	customInfo->path = NULL;
	customInfo->argc = 0;
}

/**
 * fillInfo - initializes customInfo_t struct
 * @customInfo: struct address
 * @av: argument vector
 */
void fillInfo(customInfo_t *customInfo, char **av)
{
	int i = 0;

	customInfo->fname = av[0];
	if (customInfo->arg)
	{
		customInfo->argv = split_string(customInfo->arg, " \t");
		if (!customInfo->argv)
		{

			customInfo->argv = malloc(sizeof(char *) * 2);
			if (customInfo->argv)
			{
				customInfo->argv[0] = duplicate_string(customInfo->arg);
				customInfo->argv[1] = NULL;
			}
		}
		for (i = 0; customInfo->argv && customInfo->argv[i]; i++)
			;
		customInfo->argc = i;

		replace_aliases(customInfo);
		replace_variables(customInfo);
	}
}

/**
 * releaseInfo - frees customInfo_t struct fields
 * @customInfo: struct address
 * @all: true if freeing all fields
 */
void releaseInfo(customInfo_t *customInfo, int all)
{
	free_string_array(customInfo->argv);
	customInfo->argv = NULL;
	customInfo->path = NULL;
	if (all)
	{
		if (!customInfo->cmd_buf)
			free(customInfo->arg);
		if (customInfo->env)
			freeList(&(customInfo->env));
		if (customInfo->history)
			freeList(&(customInfo->history));
		if (customInfo->alias)
			freeList(&(customInfo->alias));
		free_string_array(customInfo->environ);
		customInfo->environ = NULL;
		free_and_null((void **)customInfo->cmd_buf);
		if (customInfo->input_fd > 2)
			close(customInfo->input_fd);
		write_character(BUFFER_FLUSH);
	}
}

