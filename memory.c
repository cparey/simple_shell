#include "shell.h"

/**
 * free_and_null - frees a pointer and NULLs the address
 * @pointer_to_free: address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int free_and_null(void **pointer_to_free)
{
    if (pointer_to_free && *pointer_to_free)
    {
        free(*pointer_to_free);
        *pointer_to_free = NULL;
        return 1;
    }
    return 0;
}

