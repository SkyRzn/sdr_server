#include <module.h>
#include <routines/dbg.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>


static int set_input(instance_t *instance, instance_t *input);
static int set_output(instance_t *instance, instance_t *output);


module_t *get_module(void *dlhandle, const char *name)
{
	module_t *module;
	char fullname[FILENAME_MAX];

	dbg_assert_not_null(dlhandle, NULL);
	dbg_assert_not_null(name, NULL);

	snprintf(fullname, sizeof(fullname), "%s_%s", MODULE_PREFIX_STRING, name);

	module = dlsym(dlhandle, fullname);
	dbg_assert_not_null(module, NULL);

	return module;
}

int init_instance(instance_t *instance, const char *name)
{
	dbg_assert_not_null(instance, -EINVAL);
	dbg_assert_not_null(name, -EINVAL);

	instance->name = strdup(name);
	dbg_malloc_assert(instance->name, -ENOMEM);

	instance->module = NULL;
	instance->context = NULL;

	init_autoarray(&instance->input, instance_t);
	init_autoarray(&instance->output, instance_t);

	return 0;
}

void free_instance(instance_t *instance)
{
	free(instance->name);
}

void set_instance_module(instance_t *instance, module_t *module)
{
	dbg_assert_not_null(instance, );
	dbg_assert_not_null(module, );

	instance->module = module;

	if (module->set_input == NULL)
		module->set_input = set_input;

	if (module->set_output == NULL)
		module->set_output = set_output;
}

void set_output_instance_data(instance_t *instance, void *data, size_t offset, size_t size)
{
	instance_t *output;

	autoarray_foreach(&instance->output, output)
		output->module->handler(output, data, offset, size);
}

void begin_instance_loop(instance_t *instance)
{
	while (1) { // TODO replace by stop condition
		instance->module->handler(instance, NULL, 0, 0);
	}
}

static int set_input(instance_t *instance, instance_t *input)
{
	dbg_assert_not_null(instance, -EINVAL);
	dbg_assert_not_null(input, -EINVAL);

	return add_autoarray_item(&instance->input, input);
}

static int set_output(instance_t *instance, instance_t *output)
{
	dbg_assert_not_null(instance, -EINVAL);
	dbg_assert_not_null(output, -EINVAL);

	return add_autoarray_item(&instance->output, output);
}

/*
static void start_inpit_loop(instance_t *instance)
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
