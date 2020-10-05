#include <container.h>
#include <routines/dbg.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>


container_t *load_container(const char *name)
{
	container_t *container;
	void *dlhandle;
	char *namedup;
	char path[CONTAINER_MAX_NAME_LEN + sizeof(CONTAINER_PATH_STRING) + 8]; // 8 == len("/lib.so")

	snprintf(path, sizeof(path), "%s/lib%s.so", CONTAINER_PATH_STRING, name);

	dlhandle = dlopen(path, RTLD_LAZY);
	if (!dlhandle)
		dbg_return(NULL, "Can't open container: %s\n", dlerror());

	namedup = strdup(name);
	if (!namedup) {
		dlclose(dlhandle);
		dbg_mem_return(NULL);
	}

	container = malloc(sizeof(container_t));
	if (!container) {
		free_container(container);
		dbg_mem_return(NULL);
	}

	container->dlhandle = dlhandle;
	container->name = namedup;

	return container;
}

void free_container(container_t *container)
{
	free(container->name);
	dlclose(container->dlhandle);
}
