#include <storage.h>
#include <container.h>
#include <module.h>
#include <routines/autoarray.h>
#include <routines/dbg.h>

#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>


static autoarray_t container_array;
static autoarray_t instance_array;


static container_t *get_container(const char *name);
static const char *get_module_name(char *contname);


void init_storage(void)
{
	init_named_autoarray(&container_array, container_t, name);
	init_named_autoarray(&instance_array, module_instance_t, name);
}

void free_storage(void)
{
	container_t *container;
	module_instance_t *instance;

	autoarray_foreach(&instance_array, instance)
		free_module_instance(instance);
	free_autoarray(&instance_array);

	autoarray_foreach(&container_array, container)
		free_container(container);
	free_autoarray(&container_array);
}

module_instance_t *get_module_instance(const char *name)
{
	module_instance_t *instance;

	instance = get_autoarray_item_by_name(&instance_array, name);
	if (instance)
		return instance;

	instance = new_autoarray_item(&instance_array);
	if (!instance)
		return NULL;

	if (init_module_instance(instance, name) != 0) {
		free_module_instance(instance);
		pop_autoarray_item(&instance_array);
		return NULL;
	}

	return instance;
}

int set_instance_module(module_instance_t *instance, const char *module_name)
{
	char *contname;
	const char *modname;
	container_t *container;

	if (instance == NULL || module_name == NULL)
		dbg_return(-EINVAL, "Args error");

	contname = strdup(module_name);
	if (!contname)
		dbg_mem_return(-ENOMEM);

	modname = get_module_name(contname);

	container = get_container(contname);
	if (!container)
		dbg_return(-ENOENT, "Can't get container '%s'\n", contname);

	instance->module = get_module(container->dlhandle, modname);
	if (!instance->module)
		dbg_return(-ENOENT, "Can't get module '%s' from container '%s'\n",
				   modname, contname);

	free(contname);

	return 0;
}

static container_t *get_container(const char *name)
{
	container_t *container;

	container = get_autoarray_item_by_name(&container_array, name);
	if (container != NULL)
		return container;

	container = new_autoarray_item(&container_array);
	if (container == NULL)
		return NULL;

	int res = load_container(container, name);
	if (res != 0) {
		pop_autoarray_item(&container_array);
		return NULL;
	}

	return container;
}

static const char *get_module_name(char *contname)
{
	char *modname;

	modname = strchr(contname, ':');
	if (!modname)
		return contname;

	*modname = '\0';

	return modname + 1;
}
