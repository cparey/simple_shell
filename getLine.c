#include "shell.h"

/**
 * bufferChainedCommands - buffers chained commands
 * @customInfo: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t bufferChainedCommands(customInfo_t *customInfo, char **buf, size_t *len)
{
	ssize_t bytesRead = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, handleSIGINT);
#if USE_GETLINE
		bytesRead = getline(buf, &len_p, stdin);
#else
		bytesRead = readLine(customInfo, buf, &len_p);
#endif
		if (bytesRead > 0)
		{
			if ((*buf)[bytesRead - 1] == '\n')
			{
				(*buf)[bytesRead - 1] = '\0'; /* remove trailing newline */
				bytesRead--;
			}
			customInfo->linecount_flag = 1;
			removeComments(*buf);
			buildHistoryList(customInfo, *buf, customInfo->histcount++);
			{
				*len = bytesRead;
				customInfo->cmd_buf = buf;
			}
		}
	}
	return bytesRead;
}

/**
 * getInput - gets a line minus the newline
 * @customInfo: parameter struct
 *
 * Return: bytes read
 */
ssize_t getInput(customInfo_t *customInfo)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t bytesRead = 0;
	char **buf_p = &(customInfo->arg), *p;

	write_character(BUFFER_FLUSH);
	bytesRead = bufferChainedCommands(customInfo, &buf, &len);
	if (bytesRead == -1) /* EOF */
		return -1;
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		check_chain_continuation(customInfo, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chain_delimiter(customInfo, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			customInfo->cmd_buf_type = CHAIN_NORMAL;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return string_length(p); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from readLine() */
	return bytesRead; /* return length of buffer from readLine() */
}

/**
 * readBuffer - reads a buffer
 * @customInfo: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: bytesRead
 */
ssize_t readBuffer(customInfo_t *customInfo, char *buf, size_t *i)
{
	ssize_t bytesRead = 0;

	if (*i)
		return 0;
	bytesRead = read(customInfo->input_fd, buf, READ_BUFFER_SIZE);
	if (bytesRead >= 0)
		*i = bytesRead;
	return bytesRead;
}

/**
 * readLine - gets the next line of input from STDIN
 * @customInfo: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: bytesRead
 */
int readLine(customInfo_t *customInfo, char **ptr, size_t *length)
{
	static char buf[READ_BUFFER_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t bytesRead = 0, totalBytesRead = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		totalBytesRead = *length;
	if (i == len)
		i = len = 0;

	bytesRead = readBuffer(customInfo, buf, &len);
	if (bytesRead == -1 || (bytesRead == 0 && len == 0))
		return -1;

	c = findCharacterInString(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = custom_realloc(p, totalBytesRead, totalBytesRead ? totalBytesRead + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return p ? free(p), -1 : -1;

	if (totalBytesRead)
		concatenateStringWithLimit(new_p, buf + i, k - i);
	else
		copyStringWithLimit(new_p, buf + i, k - i + 1);

	totalBytesRead += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = totalBytesRead;
	*ptr = p;
	return totalBytesRead;
}

/**
 * handleSIGINT - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void handleSIGINT(__attribute__((unused))int sig_num)
{
	print_string("\n");
	print_string("$ ");
	write_character(BUFFER_FLUSH);
}

