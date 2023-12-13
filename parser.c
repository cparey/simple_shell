#include "shell.h"

/**
 * isExecutableCommand Checks if a file at the specified path is an executable command.
 *
 * @customInfo: customInfo The customInfo struct.
 * @param path The path to the file.
 * @return int 1 if true, 0 otherwise.
 */
int isExecutableCommand(customInfo_t *customInfo, char *path)
{
    struct stat st;

    (void)customInfo;
    if (!path || stat(path, &st))
        return 0;

    if (st.st_mode & S_IFREG)
    {
        return 1;
    }
    return 0;
}

/**
 * duplicatePathChars Duplicates characters from the specified range in the PATH string.
 *
 * @param pathstr The PATH string.
 * @param start   Starting index.
 * @param stop    Stopping index.
 * @return char*  Pointer to a new buffer.
 */
char *duplicatePathChars(char *pathstr, int start, int stop)
{
    static char buf[1024];
    int i = 0, k = 0;

    for (k = 0, i = start; i < stop; i++)
        if (pathstr[i] != ':')
            buf[k++] = pathstr[i];
    buf[k] = 0;
    return buf;
}

/**
 * findCommandPath Finds the full path of a command in the PATH string.
 *
 * @param customInfo    The customInfo struct.
 * @param pathstr The PATH string.
 * @param cmd     The command to find.
 * @return char*  Full path of the command if found, or NULL.
 */
char *findCommandPath(customInfo_t *customInfo, char *pathstr, char *cmd)
{
    int i = 0, curr_pos = 0;
    char *path;

    if (!pathstr)
        return NULL;
    if ((string_length(cmd) > 2) && string_starts_with(cmd, "./"))
    {
        if (isExecutableCommand(customInfo, cmd))
            return cmd;
    }
    while (1)
    {
        if (!pathstr[i] || pathstr[i] == ':')
        {
            path = duplicatePathChars(pathstr, curr_pos, i);
            if (!*path)
                string_concatenate(path, cmd);
            else
            {
                string_concatenate(path, "/");
                string_concatenate(path, cmd);
            }
            if (isExecutableCommand(customInfo, path))
                return path;
            if (!pathstr[i])
                break;
            curr_pos = i;
        }
        i++;
    }
    return NULL;
}

