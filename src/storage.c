#include <storage.h>
#include <container.h>
#include <module.h>
#include <routines/list.h>
#include <routines/dbg.h>

#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>


static list_t container_list;
static list_t instance_list;


static container_t *get_container(const char *name);
static const char *get_module_name(char *contname);


void init_storage(void)
{
	init_named_list(&container_list, container_t, next, name);
	init_named_list(&instance_list, module_instance_t, next, name);
}

void free_storage(void)
{
	container_t *container;
	module_instance_t *instance;

	list_foreach(&instance_list, instance)
		free_module_instance(instance);
	free_list(&instance_list);

	list_foreach(&container_list, container)
		free_container(container);
	free_list(&container_list);
}

module_instance_t *get_module_instance(const char *name)
{
	module_instance_t *instance;

	instance = find_list_item(&instance_list, name);
	if (instance)
		return instance;

	instance = create_module_instance(name);
	if (!instance)
		return NULL;

	if (add_list_item(&instance_list, instance) != 0) {
		free_module_instance(instance);
		free(instance);
		return NULL;
	}

	return instance;
}

int set_instance_module(module_instance_t *instance, const char *module_name)
{
	char *contname;
	const char *modname;
	container_t *container;

	contname = strdup(module_name);
	if (!contname)
		dbg_mem_return(-ENOMEM);

	modname = get_module_name(contname);

	container = get_container(contname);
	if (!container)
		dbg_return(-ENOENT, "Can't get container '%s'\n", contname);

	instance->module = get_module(container->dlhandle, modname);
	if (!instance->module)
		dbg_return(-ENOENT, "Can't get module '%s' fron container '%s'\n",
				   modname, contname);

	free(contname);

	return 0;
}

static container_t *get_container(const char *name)
{
	container_t *container;

	container = find_list_item(&container_list, name);
	if (container)
		return container;

	container = load_container(name);
	if (!container)
		return NULL;

	if (add_list_item(&container_list, container) != 0) {
		free_container(container);
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

