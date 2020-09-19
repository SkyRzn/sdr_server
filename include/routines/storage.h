#ifndef _SDR_SERVER_ROUTINES_STORAGE_H_
#define _SDR_SERVER_ROUTINES_STORAGE_H_


typedef struct {
	void *data;
	int size;
	int item_size;
	int item_count;
} storage_t;


#define storage_foreach(storage, iterator) \
	for (iterator = (storage)->data; \
		(void *)iterator - (storage)->data < ((storage)->item_count * (storage)->item_size); \
		iterator += (storage)->item_size)

extern void init_storage(storage_t *storage, int item_size);
extern void free_storage(storage_t *array);
extern void *new_storage_item(storage_t *storage);
extern void *get_storage_item_by_index(storage_t *storage, int index);
extern void *get_storage_item_by_name(storage_t *storage, const char *name);


#endif
