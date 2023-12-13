#include "shell.h"

/**
 * shell_loop - main shell loop
 * @customInfo: the parameter & return customInfo struct
 * @argv: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int shell_loop(customInfo_t *customInfo, char **argv)
{
    ssize_t input_length = 0;
    int builtin_ret = 0;

    while (input_length != -1 && builtin_ret != -2)
    {
        initializeInfo(customInfo);
        if (checkInteractiveMode(customInfo))
            print_string("$ ");
        writeErrorCharacter(BUFFER_FLUSH);
        input_length = getInput(customInfo);
        if (input_length != -1)
        {
            fillInfo(customInfo, argv);
            builtin_ret = findBuiltIn(customInfo);
            if (builtin_ret == -1)
                find_command(customInfo);
        }
        else if (checkInteractiveMode(customInfo))
            write_character('\n');
        releaseInfo(customInfo, 0);
    }
    writeHistory(customInfo);
    releaseInfo(customInfo, 1);
    if (!checkInteractiveMode(customInfo) && customInfo->status)
        exit(customInfo->status);
    if (builtin_ret == -2)
    {
        if (customInfo->err_num == -1)
            exit(customInfo->status);
        exit(customInfo->err_num);
    }
    return builtin_ret;
}

/**
 * findBuiltIn - finds a builtin command
 * @customInfo: the parameter & return customInfo struct
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         -2 if builtin signals exit()
 */
int findBuiltIn(customInfo_t *customInfo)
{
    int i, built_in_ret = -1;
    builtin_table builtintbl[] = {
        {"exit", exitShell},
        {"env", displayEnvironment},
        {"help", helpCommand},
        {"history", displayHistory},
        {"setenv", initializeEnvironmentVariable},
        {"unsetenv", unsetEnvironmentVariable},
        {"cd", changeDirectory},
        {"alias", manageAlias},
        {NULL, NULL}};

    for (i = 0; builtintbl[i].type; i++)
        if (string_compare(customInfo->argv[0], builtintbl[i].type) == 0)
        {
            customInfo->line_count++;
            built_in_ret = builtintbl[i].func(customInfo);
            break;
        }
    return built_in_ret;
}

/**
 * find_command - finds a command in PATH
 * @customInfo: the parameter & return customInfo struct
 *
 * Return: void
 */
void find_command(customInfo_t *customInfo)
{
    char *path = NULL;
    int i, k;

    customInfo->path = customInfo->argv[0];
    if (customInfo->linecount_flag == 1)
    {
        customInfo->line_count++;
        customInfo->linecount_flag = 0;
    }
    for (i = 0, k = 0; customInfo->arg[i]; i++)
        if (!isDelimiter(customInfo->arg[i], " \t\n"))
            k++;
    if (!k)
        return;

    path = findCommandPath(customInfo, getEnvironmentVariable(customInfo, "PATH="), customInfo->argv[0]);
    if (path)
    {
        customInfo->path = path;
        fork_command(customInfo);
    }
    else
    {
        if ((checkInteractiveMode(customInfo) || getEnvironmentVariable(customInfo, "PATH=") ||
             customInfo->argv[0][0] == '/') &&
            isExecutableCommand(customInfo, customInfo->argv[0]))
            fork_command(customInfo);
        else if (*(customInfo->arg) != '\n')
        {
            customInfo->status = 127;
            printErrorInfo(customInfo, "No such file or directory\n");
        }
    }
}

/**
 * fork_command - forks an exec thread to run command
 * @customInfo: the parameter & return customInfo struct
 *
 * Return: void
 */
void fork_command(customInfo_t *customInfo)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1)
    {
        perror("Error:");
        return;
    }
    if (child_pid == 0)
    {
        if (execve(customInfo->path, customInfo->argv, getEnvironmentStrings(customInfo)) == -1)
        {
            releaseInfo(customInfo, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
    }
    else
    {
        wait(&(customInfo->status));
        if (WIFEXITED(customInfo->status))
        {
            customInfo->status = WEXITSTATUS(customInfo->status);
            if (customInfo->status == 126)
                printErrorInfo(customInfo, "Permission denied\n");
        }
    }
}

