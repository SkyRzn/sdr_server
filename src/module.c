#include <module.h>
#include <routines/dbg.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>


module_t *get_module(void *dlhandle, const char *name)
{
	module_t **module_pointer, *module;
	char fullname[FILENAME_MAX];

	snprintf(fullname, sizeof(fullname), "%s_%s", MODULE_PREFIX_STRING, name);

	module_pointer = dlsym(dlhandle, fullname);
	if (!module_pointer)
		dbg_exit("Can't load module pointer from %s: %s\n", name, dlerror());

	module = *module_pointer;
	if (!module)
		dbg_exit("Module pointer is NULL in %s\n", name);

	if (strcmp(module->name, name) != 0)
		dbg_exit("Module name %s doesn't correspond to given %s\n", module->name, name);

	return module;
}

void init_module_instance(module_instance_t *instance, const char *name)
{
	instance->name = strdup(name);
	if (!instance->name)
		dbg_mem_exit();

	instance->module = NULL;
	instance->context = NULL;
}

void free_module_instance(module_instance_t *instance)
{
	free(instance->name);
}
