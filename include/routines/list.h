#ifndef _SDR_SERVER_ROUTINES_NAMED_LIST_H_
#define _SDR_SERVER_ROUTINES_NAMED_LIST_H_


#include <stddef.h>
#include <unistd.h>


typedef struct {
	void *first;
	void *last;
	int count;
	size_t item_size;
	size_t next_offset;
	ssize_t name_offset;
} list_t;


#ifndef offsetof
#define offsetof(_type, _member) ((size_t)&(((_type *)0)->_member))
#endif

#define item_member(_item, _offset) \
	((_item) ? ((char *)_item) + _offset : NULL)
#define next_list_item(_list, _item) \
	*(void **)item_member(_item, (_list)->next_offset)
#define list_item_name(_list, _item) \
	((_item && (_list)->name_offset >= 0) ? \
	(*(char **)item_member(_item, (_list)->name_offset)) : NULL)

#define init_list(_list, _type, _next_member) \
	_init_list(_list, sizeof(_type), offsetof(_type, _next_member), -1)
#define init_named_list(_list, _type, _next_member, _name_member) \
	_init_list(_list, sizeof(_type), offsetof(_type, _next_member), offsetof(_type, _name_member))

#define list_foreach(_list, _item) \
	for (_item = (_list)->first; \
		_item != NULL; \
		_item = next_list_item(_list, _item) \
	)

#define named_list_foreach(_list, _item, _item_name) \
	for (_item = (_list)->first, \
		_item_name = list_item_name(_list, _item); \
		_item != NULL; \
		_item = next_list_item(_list, _item), \
		_item_name = list_item_name(_list, _item) \
	)


extern void _init_list(list_t *list, size_t item_size, size_t next_offset, ssize_t name_offset);
extern void free_list(list_t *list);
extern int add_list_item(list_t *list, void *item);
extern void *find_list_item(list_t *list, const char *name);


#endif
