#include "list.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h> //TEST


void list_init(list_t *head)
{
	head->next = NULL;
}

int list_add_(list_t *head, void *data, size_t size)
{
	list_t *last;
	
	last = list_last(head);
	
	last->next = malloc(sizeof(list_t));
	if (!last->next)
		return -ENOMEM;
	
	last->next->next = NULL;
	
	if (size > 0) {
		last->next->data = malloc(size);
		printf("+++ %p\n", last->next);
		if (!last->next->data) {
			list_pop(head);
			return -ENOMEM;
		}
		*((void **)data) = last->next->data;
	} else
		last->next->data = data;
	
	return 0;
}

void *list_pop(list_t *head)
{
	list_t *it;
	
	if (!head->next)
		return NULL;
	
	list_foreach(head, it) {
		if (it->next->next == NULL) {
			void *data = it->next->data;
			free(it->next);
			it->next = NULL;
			return data;
		}
	}
	return NULL;
}

void list_clean_(list_t *head, int free_data)
{
	list_t *it, *next = head->next;
	
	while (next) {
		it = next;
		next = next->next;
		if (free_data)
			free(it->data);
		free(it);
	}
}

list_t *list_last(list_t *head)
{
	list_t *it;

	list_foreach(head, it) {
		if (it->next == NULL)
			return it;
	}
	
	return head;
}
