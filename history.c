#include "shell.h"

/**
 * getHistoryFile - gets the history file
 * @customInfo: parameter struct
 *
 * Return: allocated string containing history file
 */
char *getHistoryFile(customInfo_t *customInfo)
{
	char *buf, *dir;

	dir = getEnvironmentVariable(customInfo, "HOME=");
	if (!dir)
		return NULL;
	buf = malloc(sizeof(char) * (string_length(dir) + string_length(HIST_FILE) + 2));
	if (!buf)
		return NULL;
	buf[0] = 0;
	copy_string(buf, dir);
	string_concatenate(buf, "/");
	string_concatenate(buf, HIST_FILE);
	return buf;
}

/**
 * writeHistory - creates a file, or appends to an existing file
 * @customInfo: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int writeHistory(customInfo_t *customInfo)
{
	ssize_t fd;
	char *filename = getHistoryFile(customInfo);
	list_t *node = NULL;

	if (!filename)
		return -1;

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return -1;
	for (node = customInfo->history; node; node = node->next)
	{
		printStringToFD(node->str, fd);
		writeCharacterToFD('\n', fd);
	}
	writeCharacterToFD(BUFFER_FLUSH, fd);
	close(fd);
	return 1;
}

/**
 * readHistory - reads history from file
 * @customInfo: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int readHistory(customInfo_t *customInfo)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = getHistoryFile(customInfo);

	if (!filename)
		return 0;

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return 0;
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return 0;
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return 0;
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return free(buf), 0;
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			buildHistoryList(customInfo, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		buildHistoryList(customInfo, buf + last, linecount++);
	free(buf);
	customInfo->histcount = linecount;
	while (customInfo->histcount-- >= HIST_MAX)
		deleteNodeAtIndex(&(customInfo->history), 0);
	renumberHistory(customInfo);
	return customInfo->histcount;
}

/**
 * buildHistoryList - adds entry to a history linked list
 * @customInfo: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int buildHistoryList(customInfo_t *customInfo, char *buf, int linecount)
{
	list_t *node = NULL;

	if (customInfo->history)
		node = customInfo->history;
	addNodeEnd(&node, buf, linecount);

	if (!customInfo->history)
		customInfo->history = node;
	return 0;
}

/**
 * renumberHistory - renumbers the history linked list after changes
 * @customInfo: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumberHistory(customInfo_t *customInfo)
{
	list_t *node = customInfo->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (customInfo->histcount = i);
}

