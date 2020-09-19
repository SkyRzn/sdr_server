#include "list.h"
#include "test.h"
#include "../src/list.h"
#include <stdio.h> //TEST


int test_list()
{
	list_t head, *it;
	int *val;
	
	test_start_block("list");
	
	list_init(&head);
	
	for (int i = 0; i < 10; i++) {
		test_assert(list_add_alloc(&head, &val, sizeof(int)) == 0, "ENOMEM");
		*val = i;
	}
	
	test_assert(list_add(&head, "abc") == 0, "ENOMEM");
	printf("-----------------------\n");
	
	list_foreach(&head, it) {
		printf("+++ %d %p\n", *((int*)it->data), it);
	}
	
	return 0;
}

