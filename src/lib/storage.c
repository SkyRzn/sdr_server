#include <storage.h>
#include <container.h>
#include <module.h>
#include <routines/dbg.h>

#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>


static autoarray_t container_array;
static autoarray_t instance_array;
static autoarray_t thread_array;


static container_t *get_container(const char *name);
static const char *get_module_name(char *contname);


void init_storage(void)
{
	init_named_autoarray(&container_array, container_t, name);
	init_named_autoarray(&instance_array, instance_t, name);
	init_autoarray(&thread_array, thread_t);
}

void free_storage(void)
{
	container_t *container;
	instance_t *instance;

	autoarray_foreach(&instance_array, instance)
		free_instance(instance);
	free_autoarray(&instance_array);

	autoarray_foreach(&container_array, container)
		free_container(container);
	free_autoarray(&container_array);

	free_autoarray(&thread_array);
}

instance_t *get_instance(const char *name)
{
	instance_t *instance;

	instance = get_autoarray_item_by_name(&instance_array, name);
	if (instance)
		return instance;

	instance = new_autoarray_item(&instance_array);
	if (!instance)
		return NULL;

	if (init_instance(instance, name) != 0) {
		free_instance(instance);
		pop_autoarray_item(&instance_array);
		return NULL;
	}

	return instance;
}

int set_instance_module(instance_t *instance, const char *module_name)
{
	char *contname;
	const char *modname;
	container_t *container;

	dbg_assert_not_null(instance, -EINVAL);
	dbg_assert_not_null(module_name, -EINVAL);

	contname = strdup(module_name);
	dbg_malloc_assert(contname, -ENOMEM);

	modname = get_module_name(contname);

	container = get_container(contname);
	dbg_assert(container != NULL, -ENOENT, "Can't get container '%s'\n", contname);

	instance->module = get_module(container->dlhandle, modname);
	dbg_assert(instance->module != NULL, -ENOENT,
			   "Can't get module '%s' from container '%s'\n", modname, contname);

	free(contname);

	return 0;
}

autoarray_t *_instance_array(void)
{
	return &instance_array;
}

autoarray_t *_thread_array(void)
{
	return &thread_array;
}

thread_t *new_thread(void)
{
	return new_autoarray_item(&thread_array);
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

	if (load_container(container, name) != 0) {
		container = pop_autoarray_item(&container_array);
		free(container);
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
