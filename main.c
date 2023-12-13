#include "shell.h"
#include <string.h>  /* Add this line to include the necessary header for memset*/

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
    customInfo_t customInfo[] = {INFO_INIT};
    int fd = 2;

    fd += 3;

    if (ac == 2)
    {
        fd = open(av[1], O_RDONLY);
        if (fd == -1)
        {
            if (errno == EACCES)
                exit(126);
            if (errno == ENOENT)
            {
                printErrorString(av[0]);
                printErrorString(": 0: Can't open ");
                printErrorString(av[1]);
                writeErrorCharacter('\n');
                writeErrorCharacter(BUFFER_FLUSH);
                exit(127);
            }
            return (EXIT_FAILURE);
        }
        customInfo->input_fd = fd;
    }
    populateEnvironmentList(customInfo);
    readHistory(customInfo);
    shell_loop(customInfo, av);
    return (EXIT_SUCCESS);
}

