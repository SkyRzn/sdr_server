#include <routines/storage.h>
#include <routines/dbg.h>

#include <stdlib.h>
#include <string.h>


void init_storage(storage_t *storage, int item_size)
{
	storage->data = NULL;
	storage->size = 0;
	storage->item_size = item_size;
	storage->item_count = 0;
}

void free_storage(storage_t *storage)
{
	free(storage->data);
	init_storage(storage, 0);
}

void *new_storage_item(storage_t *storage)
{
	int offset;
	void *item;

	offset = storage->item_size * storage->item_count;

	if (storage->size == 0) {
		storage->size = storage->item_size;
		storage->data = malloc(storage->item_size);
		if (!storage->data)
			dbg_mem_exit();
		item = storage->data;
	} else if (offset + storage->item_size <= storage->size) {
		item = storage->data + offset;
	} else {
		storage->size *= 2;
		storage->data = realloc(storage->data, storage->size);
		if (!storage->data)
			dbg_mem_exit();
		item = storage->data + (storage->item_size * storage->item_count);
	}

	storage->item_count++;

	return item;
}

void *get_storage_item_by_index(storage_t *storage, int index)
{
	if (index < 0 || index >= storage->item_count)
		return NULL;

	return storage->data + storage->item_size * index;
}

void *get_storage_item_by_name(storage_t *storage, const char *name)
{
	int i, offset;
	char *p;

	for (i = 0, offset = 0; i < storage->item_count; i++) {
		p = (char *)(storage->data + offset);

		if (strcmp(p, name) == 0)
			return p;

		offset += storage->item_size;
	}

	return NULL;
}


