#include <module.h>
#include <routines/autoarray.h> //TEST

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>


#define LEN 1024
static int DATA[LEN];


static void source_handler(instance_t *instance, void *data, size_t offset, size_t size)
{
	int i;
	autoarray_t arr;

	(void)data;
	(void)offset;
	(void)size;

	init_autoarray(&arr, autoarray_t); //TEST

	for (i = 0; i < LEN; i++)
		DATA[i] = i;

	set_output_instance_data(instance, data, 0, LEN * sizeof(int));
}

static void doubling_handler(instance_t *instance, void *data_, size_t offset, size_t size)
{
	int i, *data = (int *)data_;

	for (i = offset/sizeof(int); i < (offset + size)/sizeof(int); i++)
		data[i] *= 2;

	set_output_instance_data(instance, data, offset, size);
}

static void destination_handler(instance_t *instance, void *data_, size_t offset, size_t size)
{
	int i, *data = (int *)data_;

	for (i = offset/sizeof(int); i < (offset + size)/sizeof(int); i++) {
		(void)data; // check
	}
}


ADD_MODULE(source,
		   NULL,
		   NULL,
		   source_handler,
		   NULL,
		   NULL);

ADD_MODULE(doubling,
		   NULL,
		   NULL,
		   doubling_handler,
		   NULL,
		   NULL);

ADD_MODULE(destination,
		   NULL,
		   NULL,
		   destination_handler,
		   NULL,
		   NULL);
