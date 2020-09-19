#ifndef _SDR_SERVER_LIST_H_
#define _SDR_SERVER_LIST_H_


#include <stddef.h>


typedef struct list {
	struct list *next;
	void *data;
} list_t;


void list_init(list_t *head);
int list_add_(list_t *head, void *data, size_t size);
void list_clean_(list_t *head, int free_data);
void *list_pop(list_t *head);
list_t *list_last(list_t *head);

#define list_foreach(head, it) for (it = (head)->next; it; it = it->next)
#define list_add(head, data) list_add_(head, (void*)data, 0)
#define list_add_alloc(head, ptr, size) list_add_(head, ptr, size)
#define list_clean(head) list_clean_(head, 0)
#define list_clean_free(head) list_clean_(head, 1)


#endif
