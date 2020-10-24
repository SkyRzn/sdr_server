#include <module.h>
#include <routines/dbg.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>


module_t *get_module(void *dlhandle, const char *name)
{
	module_t *module;
	char fullname[FILENAME_MAX];

	dbg_assert_not_null(dlhandle, NULL);
	dbg_assert_not_null(name, NULL);

	snprintf(fullname, sizeof(fullname), "%s_%s", MODULE_PREFIX_STRING, name);

	module = dlsym(dlhandle, fullname);
	dbg_assert_not_null(module, NULL);

// 	dbg_assert(strcmp(module->name, name) == 0,
// 		NULL, "Module name %s doesn't correspond to given %s\n", module->name, name); // TODO remove module->name and this assertion

	return module;
}

int init_module_instance(module_instance_t *instance, const char *name)
{
	dbg_assert_not_null(instance, -EINVAL);
	dbg_assert_not_null(name, -EINVAL);

	instance->name = strdup(name);
	dbg_malloc_assert(instance->name, -ENOMEM);

	instance->module = NULL;
	instance->data = NULL;
	instance->data_size = 0;
	instance->context = NULL;
	instance->input = NULL;
	instance->output = NULL;

	return 0;
}

void free_module_instance(module_instance_t *instance)
{
	free(instance->name);
}

void set_output_instance_module_data(module_instance_t *instance, void *data, size_t size)
{
	instance->output->data = data;
	instance->output->data_size = size;
	instance->output->module->handler(instance->output);
}

void begin_module_instance_loop(module_instance_t *instance)
{
	while (1) { // TODO replace by stop condition
		instance->module->handler(instance);
	}
}

/*
static void start_inpit_loop(module_instance_t *instance)
{
	module_handler_t *child;

	while (1) {
		// TODO lock self in

		// ... parent writes data pointer and data_size to this instance
		// ... parent unlocks this in

		instance->module->handler(instance);

		// ... children lock it's in

		// TODO write data pointer and data_size to child instance

		autoarray_foreach(&instance->output_instance_array, child) {
			// TODO unlock child in
		}

		// ... child handles it's data

		// TODO unlock parent out

		// TODO lock self out
	}
}
*/
