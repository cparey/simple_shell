#include "shell.h"

/**
 * addNode - adds a node to the start of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
list_t *addNode(list_t **head, const char *str, int num)
{
	list_t *newHead;

	if (!head)
		return NULL;
	newHead = malloc(sizeof(list_t));
	if (!newHead)
		return NULL;
	_mem_set((void *)newHead, 0, sizeof(list_t));
	newHead->num = num;
	if (str)
	{
		newHead->str = duplicate_string(str);
		if (!newHead->str)
		{
			free(newHead);
			return NULL;
		}
	}
	newHead->next = *head;
	*head = newHead;
	return newHead;
}

/**
 * addNodeEnd - adds a node to the end of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
list_t *addNodeEnd(list_t **head, const char *str, int num)
{
	list_t *newNode, *node;

	if (!head)
		return NULL;

	node = *head;
	newNode = malloc(sizeof(list_t));
	if (!newNode)
		return NULL;
	_mem_set((void *)newNode, 0, sizeof(list_t));
	newNode->num = num;
	if (str)
	{
		newNode->str = duplicate_string(str);
		if (!newNode->str)
		{
			free(newNode);
			return NULL;
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = newNode;
	}
	else
		*head = newNode;
	return newNode;
}

/**
 * printListStr - prints only the str element of a list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t printListStr(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		print_string(h->str ? h->str : "(nil)");
		print_string("\n");
		h = h->next;
		i++;
	}
	return i;
}

/**
 * deleteNodeAtIndex - deletes node at given index
 * @head: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int deleteNodeAtIndex(list_t **head, unsigned int index)
{
	list_t *node, *prevNode;
	unsigned int i = 0;

	if (!head || !*head)
		return 0;

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return 1;
	}
	node = *head;
	while (node)
	{
		if (i == index)
		{
			prevNode->next = node->next;
			free(node->str);
			free(node);
			return 1;
		}
		i++;
		prevNode = node;
		node = node->next;
	}
	return 0;
}

/**
 * freeList - frees all nodes of a list
 * @headPtr: address of pointer to head node
 *
 * Return: void
 */
void freeList(list_t **headPtr)
{
	list_t *node, *nextNode, *head;

	if (!headPtr || !*headPtr)
		return;
	head = *headPtr;
	node = head;
	while (node)
	{
		nextNode = node->next;
		free(node->str);
		free(node);
		node = nextNode;
	}
	*headPtr = NULL;
}

