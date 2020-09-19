#include <main_storage.h>
#include <container.h>
#include <module.h>
#include <routines/storage.h>
#include <routines/dbg.h>

#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>


static storage_t container_storage;
static storage_t instance_storage;


static container_t *get_container(const char *name);
static const char *get_module_name(char *contname);


void init_main_storage(void)
{
	init_storage(&container_storage, sizeof(container_t));
	init_storage(&instance_storage, sizeof(module_instance_t));
}

void free_main_storage(void)
{
	container_t *container;
	module_instance_t *instance;

	storage_foreach(&instance_storage, instance)
		free_module_instance(instance);
	free_storage(&instance_storage);

	storage_foreach(&container_storage, container)
		free_container(container);
	free_storage(&container_storage);
}

module_instance_t *get_module_instance(const char *name)
{
	module_instance_t *instance;

	instance = (module_instance_t *)get_storage_item_by_name(&instance_storage, name);
	if (instance)
		return instance;

	instance = (module_instance_t *)new_storage_item(&instance_storage);

	init_module_instance(instance, name);

	return instance;
}

void set_instance_module(module_instance_t *instance, const char *name)
{
	char *contname;
	const char *modname;
	container_t *container;

	contname = strdup(name);
	if (!contname)
		dbg_mem_exit();

	modname = get_module_name(contname);

	container = get_container(contname);

	instance->module = get_module(container->dlhandle, modname);
}

static container_t *get_container(const char *name)
{
	container_t *container;

	container = (container_t *)get_storage_item_by_name(&container_storage, name);
	if (container)
		return container;

	container = new_storage_item(&container_storage);

	init_container(container, name);

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

