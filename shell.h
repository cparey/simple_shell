#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

/* for command chaining */
#define CHAIN_NORMAL	0
#define CHAIN_OR		1
#define CHAIN_AND		2
#define CHAIN_CUSTOM	3

/* Conversion Flags for convertNumberToString()*/
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* Feature Flags for System Functions */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".custom_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@input_fd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int input_fd;
	int histcount;
} customInfo_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(customInfo_t *);
} builtin_table;


/* shell_main_loop.c */
int shell_loop(customInfo_t *, char **);
int findBuiltIn(customInfo_t *);
void find_command(customInfo_t *);
void fork_command(customInfo_t *);

/* parser.c */
int isExecutableCommand(customInfo_t *, char *);
char *duplicatePathChars(char *, int, int);
char *findCommandPath(customInfo_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* errors.c */
void printErrorString(char *);
int writeErrorCharacter(char);
int writeCharacterToFD(char c, int fd);
int printStringToFD(char *str, int fd);

/* string.c */
int string_length(char *);
int string_compare(char *, char *);
char *string_starts_with(const char *, const char *);
char *string_concatenate(char *, char *);

/* string1.c */
char *copy_string(char *, char *);
char *duplicate_string(const char *);
void print_string(char *);
int write_character(char);

/* exits.c */
char *copyStringWithLimit(char *, char *, int);
char *concatenateStringWithLimit(char *, char *, int);
char *findCharacterInString(char *, char);

/* tokenizer.c */
char **split_string(char *, char *);
char **split_string_with_delimiter(char *, char);

/* realloc.c */
char *_mem_set(char *, char, unsigned int);
void free_string_array(char **);
void *custom_realloc(void *, unsigned int, unsigned int);

/* memory.c */
int free_and_null(void **);

/* atoi.c */
int checkInteractiveMode(customInfo_t *);
int isDelimiter(char, char *);
int is_alpha(int);
int string_to_integer(char *);

/* errors1.c */
int safeAtoi(char *);
void printErrorInfo(customInfo_t *, char *);
int printInteger(int, int);
char *convertNumberToString(long int, int, int);
void removeComments(char *);

/* builtin.c */
int exitShell(customInfo_t *);
int changeDirectory(customInfo_t *);
int helpCommand(customInfo_t *);

/* builtin1.c */
int displayHistory(customInfo_t *);
int manageAlias(customInfo_t *);

/* getline.c */
ssize_t getInput(customInfo_t *);
int readLine(customInfo_t *, char **, size_t *);
void handleSIGINT(int);

/* getinfo.c */
void initializeInfo(customInfo_t *);
void fillInfo(customInfo_t *, char **);
void releaseInfo(customInfo_t *, int);

/* environ.c */
char *getEnvironmentVariable(customInfo_t *, const char *);
int displayEnvironment(customInfo_t *);
int initializeEnvironmentVariable(customInfo_t *);
int unsetEnvironmentVariable(customInfo_t *);
int populateEnvironmentList(customInfo_t *);

/* getenv.c */
char **getEnvironmentStrings(customInfo_t *);
int remEnvironmentVariable(customInfo_t *, char *);
int setEnvironmentVariable(customInfo_t *, char *, char *);

/* history.c */
char *getHistoryFile(customInfo_t *customInfo);
int writeHistory(customInfo_t *customInfo);
int readHistory(customInfo_t *customInfo);
int buildHistoryList(customInfo_t *customInfo, char *buf, int linecount);
int renumberHistory(customInfo_t *customInfo);

/* lists.c */
list_t *addNode(list_t **, const char *, int);
list_t *addNodeEnd(list_t **, const char *, int);
size_t printListStr(const list_t *);
int deleteNodeAtIndex(list_t **, unsigned int);
void freeList(list_t **);

/* lists1.c */
size_t listLength(const list_t *);
char **listToStrings(list_t *);
size_t printList(const list_t *);
list_t *nodeStartsWith(list_t *, char *, char);
ssize_t getNodeIndex(list_t *, list_t *);

/* vars.c */
int is_chain_delimiter(customInfo_t *, char *, size_t *);
void check_chain_continuation(customInfo_t *, char *, size_t *, size_t, size_t);
int replace_aliases(customInfo_t *);
int replace_variables(customInfo_t *);
int replace_string(char **, char *);

#endif

