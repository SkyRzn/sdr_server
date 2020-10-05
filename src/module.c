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

module_instance_t *create_module_instance(const char *name)
{
	module_instance_t *instance;

	instance = malloc(sizeof(module_instance_t));
	if (!instance)
		dbg_mem_return(NULL);

	instance->name = strdup(name);
	if (!instance->name) {
		free(instance);
		dbg_mem_return(NULL);
	}

	instance->module = NULL;
	instance->context = NULL;

	return instance;
}

void free_module_instance(module_instance_t *instance)
{
	free(instance->name);
}
