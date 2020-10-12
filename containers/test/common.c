#include <module.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>


#define LEN 1024
static int data[LEN];


static void source_handler(module_instance_t *instance)
{
	int i;

	for (i = 0; i < LEN; i++)
		data[i] = i;

	set_output_instance_module_data(instance, data, LEN * sizeof(int));
}

static void doubling_handler(module_instance_t *instance)
{
	int i;

	for (i = 0; i < instance->data_size/sizeof(int); i++)
		data[i] *= 2;

	set_output_instance_module_data(instance, instance->data, instance->data_size);
}

static void destination_handler(module_instance_t *instance)
{
	int i;

	for (i = 0; i < instance->data_size/sizeof(int); i++) {
		if (data[i] != i * 2)
			exit(-EINVAL);
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
