#include <routines/autoarray.h>
#include <routines/dbg.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>


int _init_autoarray(autoarray_t *array, size_t item_size, ssize_t name_offset)
{
	if (array == NULL)
		dbg_return(-EINVAL, "Array arg is NULL\n");

	if (item_size < 0)
		dbg_return(-EINVAL, "Item size is less than zero\n");

	array->data = NULL;
	array->size = 0;
	array->used_size = 0;
	array->item_size = item_size;
	array->name_offset = name_offset;
	array->item_count = 0;

	return 0;
}

void free_autoarray(autoarray_t *array)
{
	if (array == NULL)
			dbg_return(, "Array arg is NULL\n");

	free(array->data);
	_init_autoarray(array, 0, -1);
}

void *new_autoarray_item(autoarray_t *array)
{
	void *item;

	if (array == NULL)
		dbg_return(NULL, "Array arg is NULL\n");

	if (array->size == 0) {
		array->data = malloc(array->item_size);
		if (!array->data)
			dbg_mem_return(NULL);
		array->size = array->item_size;
	} else if (array->used_size + array->item_size > array->size) {
		array->size *= 2;
		array->data = realloc(array->data, array->size);
		if (!array->data)
			dbg_mem_return(NULL);
	}

	item = array->data + array->used_size;
	array->used_size += array->item_size;
	array->item_count++;

	return item;
}

void *get_autoarray_item_by_index(autoarray_t *array, int index)
{
	if (array == NULL)
		dbg_return(NULL, "Array arg is NULL\n");

	if (index < 0 || index >= array->item_count)
		dbg_return(NULL, "Incorrect array index: %d\n", index);

	return array->data + array->item_size * index;
}

void *get_autoarray_item_by_name(autoarray_t *array, const char *name)
{
	const char *item_name;
	void *item;

	if (array == NULL || name == NULL)
		dbg_return(NULL, "Argument is NULL\n");

	if (array->name_offset < 0) {
		dbg_return(NULL, "This array isn't a named array\n");
		return NULL;
	}

	autoarray_foreach(array, item) {
		item_name = autoarray_item_name(array, item);
		if (strcmp(item_name, name) == 0)
			return item;
	}

	return NULL;
}

void *pop_autoarray_item(autoarray_t *array)
{
	if (array == NULL)
		dbg_return(NULL, "Array arg is NULL\n");

	if (array->item_count < 1)
		dbg_return(NULL, "Autoarray is empty\n");

	array->item_count -= 1;
	array->used_size -= array->item_size;

	return (array->data + array->used_size);
}
