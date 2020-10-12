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

	if (dlhandle == NULL) {
		dbg("Arg dlhandle is NULL\n");
		return NULL;
	}

	if (name == NULL) {
		dbg("Arg name is NULL\n");
		return NULL;
	}

	snprintf(fullname, sizeof(fullname), "%s_%s", MODULE_PREFIX_STRING, name);

	module = dlsym(dlhandle, fullname);
	if (!module) {
		dbg("Module pointer is NULL in %s\n", name);
		return NULL;
	}

	if (strcmp(module->name, name) != 0) {
		dbg("Module name %s doesn't correspond to given %s\n", module->name, name);
		return NULL;
	}

	return module;
}

int init_module_instance(module_instance_t *instance, const char *name)
{
	int res;

	if (!instance || !name)
		dbg_return(-EINVAL, "Argument is NULL\n");

	res = init_autoarray(&instance->input_instance_autoarray, module_instance_t);
	if (res != 0)
		return res;
	instance->input = instance->input_instance_autoarray.data;

	res = init_autoarray(&instance->output_instance_autoarray, module_instance_t);
	if (res != 0)
		return res;
	instance->output = instance->output_instance_autoarray.data;

	instance->name = strdup(name);
	if (!instance->name)
		dbg_mem_return(-ENOMEM);

	instance->module = NULL;
	instance->data = NULL;
	instance->data_size = 0;
	instance->context = NULL;

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
