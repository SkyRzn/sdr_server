#include <routines/list.h>
#include <routines/dbg.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>


void _init_list(list_t *list, size_t item_size, size_t next_offset, ssize_t name_offset)
{
	if (!list) {
		dbg("List pointer is NULL\n");
		return;
	}

	list->first = NULL;
	list->last = NULL;
	list->count = 0;
	list->item_size = item_size;
	list->next_offset = next_offset;
	list->name_offset = name_offset;
}

void free_list(list_t *list)
{
	void *item, *next;

	if (!list)
		dbg_return(, "List pointer is NULL\n");

	for (item = list->first; item != NULL; item = next) {
		next = next_list_item(list, item);
		free(item);
	}
}

int add_list_item(list_t *list, void *item)
{
	if (!list)
		dbg_return(-EINVAL, "List pointer is NULL\n");

	if (!item)
		dbg_return(-EINVAL, "Item pointer is NULL\n");

	if (!list->first)
		list->first = item;
	else
		next_list_item(list, list->last) = item;

	list->last = item;

	next_list_item(list, item) = NULL;

	list->count++;

	return 0;
}

void *find_list_item(list_t *list, const char *name)
{
	void *item;
	const char *item_name;

	if (list->name_offset < 0)
		dbg_return(NULL, "Searching in unnamed list\n");

	named_list_foreach(list, item, item_name) {
		if (strcmp(item_name, name) == 0)
			return item;
	}

	return NULL;
}
