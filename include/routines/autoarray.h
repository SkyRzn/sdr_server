#ifndef _SDR_SERVER_ROUTINES_AUTOARRAY_H_
#define _SDR_SERVER_ROUTINES_AUTOARRAY_H_


#include <sys/types.h>
#include <stddef.h>


typedef struct {
	int count;
	size_t size;
	size_t item_size;
	ssize_t name_offset;

	void **pointer;

	int index;
} autoarray_t;


#define init_autoarray(_array, _type) \
	_init_autoarray(_array, sizeof(_type), -1)

#define init_named_autoarray(_array, _type, _name_member) \
	_init_autoarray(_array, sizeof(_type), offsetof(_type, _name_member))

#define autoarray_item_member(_item, _offset) \
	((_item) ? ((void *)_item) + _offset : NULL)

#define autoarray_item_name(_array, _item) \
	((_item && (_array)->name_offset >= 0) ? \
	(*(char **)autoarray_item_member(_item, (_array)->name_offset)) : NULL)

#define autoarray_foreach(_array, _item) \
	for (_item = init_autoarray_foreach(_array); \
		_item != NULL; \
		_item = push_autoarray_foreach(_array))

extern int _init_autoarray(autoarray_t *array, size_t item_size, ssize_t name_offset);
extern void free_autoarray(autoarray_t *array);
extern void *new_autoarray_item(autoarray_t *array);
extern int add_autoarray_item(autoarray_t *array, void *item);
extern void *get_autoarray_item_by_index(autoarray_t *array, int index);
extern void *get_autoarray_item_by_name(autoarray_t *array, const char *name);
extern void *pop_autoarray_item(autoarray_t *array);
extern void *init_autoarray_foreach(autoarray_t *array);
extern void *push_autoarray_foreach(autoarray_t *array);


#endif
