#include <routines/autoarray.h>
#include <routines/dbg.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h> //TEST


#define POINTER_SIZE sizeof(void *)


static void **add_pointer(autoarray_t *array);


int _init_autoarray(autoarray_t *array, size_t item_size, ssize_t name_offset)
{
	dbg_assert_not_null(array, -EINVAL);

	if (item_size < 0)
		dbg_return(-EINVAL, "Item size is less than zero\n");

	array->pointer = malloc(POINTER_SIZE);
	dbg_malloc_assert(array, -ENOMEM);

	array->size = POINTER_SIZE;
	array->item_size = item_size;
	array->name_offset = name_offset;
	array->count = 0;
	array->index = 0;

	return 0;
}

void free_autoarray(autoarray_t *array)
{
	void *item;

	dbg_assert_not_null(array, );

	if (array->item_size > 0) {
		autoarray_foreach(array, item)
			free(item);
	}

	free(array->pointer);

	_init_autoarray(array, 0, -1);
}

void *new_autoarray_item(autoarray_t *array)
{
	void **pointer;

	dbg_assert_not_null(array, NULL);
	dbg_assert(array->item_size != 0, NULL, "New autoarray item with zero size\n");

	pointer = add_pointer(array);
	dbg_assert_not_null(pointer, NULL);

	*pointer = malloc(array->item_size);
	dbg_assert_not_null(*pointer, NULL);

	return *pointer;
}

int add_autoarray_item(autoarray_t *array, void *item)
{
	void **pointer;

	dbg_assert_not_null(array, -EINVAL);
	dbg_assert_not_null(item, -EINVAL);
	dbg_assert(array->item_size == 0, -EINVAL, "Add autoarray item with not zero size\n");

	pointer = add_pointer(array);
	dbg_assert_not_null(pointer, -EINVAL);

	*pointer = item;

	return 0;
}

void *get_autoarray_item_by_index(autoarray_t *array, int index)
{
	dbg_assert_not_null(array, NULL);

	if (index < 0 || index >= array->count)
		dbg_return(NULL, "Incorrect array index: %d\n", index);

	return *(array->pointer + index);
}

void *get_autoarray_item_by_name(autoarray_t *array, const char *name)
{
	const char *item_name;
	void *item;

	dbg_assert_not_null(array, NULL);
	dbg_assert_not_null(name, NULL);

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
	dbg_assert_not_null(array, NULL);

	if (array->count < 1)
		dbg_return(NULL, "Autoarray is empty\n");

	array->count--;

	return *(array->pointer + array->count);
}

void *init_autoarray_foreach(autoarray_t *array)
{
	if (array->count < 1)
		return NULL;

	array->index = 0;

	return *(array->pointer + array->index);
}

void *push_autoarray_foreach(autoarray_t *array)
{
	if (++array->index >= array->count)
		return NULL;

	return *(array->pointer + array->index);
}

static void **add_pointer(autoarray_t *array)
{
	void **pointer;

	dbg_assert_not_null(array, NULL);

	if ((array->count + 1) * POINTER_SIZE > array->size) {
		array->size *= 2;
		array->pointer = realloc(array->pointer, array->size);
		dbg_assert_not_null(array->pointer, NULL);
	}

	pointer = array->pointer + array->count;
	array->count++;

	return pointer;
}
