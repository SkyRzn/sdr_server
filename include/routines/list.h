#ifndef _SDR_SERVER_ROUTINES_LIST_H_
#define _SDR_SERVER_ROUTINES_LIST_H_


typedef struct list_item_t list_item_t;


struct list_item_t
{
	list_item_t *prev;
	list_item_t *next;

	void *data;
};

void list_init(list_item_t *head);
list_item_t *list_add(list_item_t *head, void *data);
void list_del(list_item_t *item);
void list_clear(list_item_t *head);
void list_clear_free(list_item_t *head);
int list_count(list_item_t *head);


#define list_init_new(head) {head = (list_item_t *)malloc(sizeof(list_item_t)); list_init(head);}
#define list_add_new(head, data, type) ({data = (type *)malloc(sizeof(type)); list_add(head, data);})
#define list_del_free(item) {free(item->data); list_del(item);}
#define list_data(list_item, type) ((type *)(list_item)->data)
#define list_for_each(head, item) for(item = (head)->next; item != head; item = item->next)


#endif
