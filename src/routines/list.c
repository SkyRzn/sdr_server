#include <routines/list.h>
#include <stdlib.h>


void list_init(list_item_t *head)
{
	if (head)
	{
		head->prev = head;
		head->next = head;
		head->data = NULL;
	}
}

list_item_t *list_add(list_item_t *head, void *data)
{
	list_item_t *it;

	if (!data)
		return NULL;

	it = (list_item_t *)malloc(sizeof(list_item_t));

	if (it)
	{
		it->prev = head->prev;
		it->next = head;
		head->prev->next = it;
		head->prev = it;

		it->data = data;
	}
	else
		free(data);

	return it;
}

void list_del(list_item_t *item)
{
	if (item)
	{
		item->prev->next = item->next;
		item->next->prev = item->prev;
		free(item);
	}
}

void list_clear(list_item_t *head)
{
	list_item_t *item, *next_item;
	item = head->next;
	while (item != head)
	{
		next_item = item->next;
		free(item);
		item = next_item;
	}
	list_init(head);
}

void list_clear_free(list_item_t *head)
{
	list_item_t *item, *next_item;
	item = head->next;
	while (item != head)
	{
		next_item = item->next;
		free(item->data);
		free(item);
		item = next_item;
	}
	list_init(head);
}

int list_count(list_item_t *head)
{
	list_item_t *item;
	int count = 0;

	list_for_each(head, item)
		count++;

	return count;
}
