#include "shell.h"

/**
 * exitShell - exits the shell
 * @customInfo: Structure containing potential arguments.
 * Return: Exits with a given exit status (0) if customInfo->argv[0] != "exit"
 */
int exitShell(customInfo_t *customInfo)
{
	int checkExit;

	if (customInfo->argv[1])  /* If there is an exit argument */
	{
		checkExit = safeAtoi(customInfo->argv[1]);
		if (checkExit == -1)
		{
			customInfo->status = 2;
			printErrorInfo(customInfo, "Illegal number: ");
			printErrorString(customInfo->argv[1]);
			writeErrorCharacter('\n');
			return (1);
		}
		customInfo->err_num = safeAtoi(customInfo->argv[1]);
		return (-2);
	}
	customInfo->err_num = -1;
	return (-2);
}

/**
 * changeDirectory - changes the current directory of the process
 * @customInfo: Structure containing potential arguments.
 * Return: Always 0
 */
int changeDirectory(customInfo_t *customInfo)
{
	char *currentDir, *newDir, buffer[1024];
	int chdirRet;

	currentDir = getcwd(buffer, 1024);
	if (!currentDir)
		print_string("TODO: >>getcwd failure emsg here<<\n");
	if (!customInfo->argv[1])
	{
		newDir = getEnvironmentVariable(customInfo, "HOME=");
		if (!newDir)
			chdirRet = /* TODO: what should this be? */
				chdir((newDir = getEnvironmentVariable(customInfo, "PWD=")) ? newDir : "/");
		else
			chdirRet = chdir(newDir);
	}
	else if (string_compare(customInfo->argv[1], "-") == 0)
	{
		if (!getEnvironmentVariable(customInfo, "OLDPWD="))
		{
			print_string(currentDir);
			write_character('\n');
			return (1);
		}
		print_string(getEnvironmentVariable(customInfo, "OLDPWD=")), write_character('\n');
		chdirRet = /* TODO: what should this be? */
			chdir((newDir = getEnvironmentVariable(customInfo, "OLDPWD=")) ? newDir : "/");
	}
	else
		chdirRet = chdir(customInfo->argv[1]);
	if (chdirRet == -1)
	{
		printErrorInfo(customInfo, "can't cd to ");
		printErrorString(customInfo->argv[1]), writeErrorCharacter('\n');
	}
	else
	{
		setEnvironmentVariable(customInfo, "OLDPWD", getEnvironmentVariable(customInfo, "PWD="));
		setEnvironmentVariable(customInfo, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * helpCommand - displays help information
 * @customInfo: Structure containing potential arguments.
 * Return: Always 0
 */
int helpCommand(customInfo_t *customInfo)
{
	char **argArray;

	argArray = customInfo->argv;
	print_string("help call works. Function not yet implemented \n");
	if (0)
		print_string(*argArray); /* temp att_unused workaround */
	return (0);
}

