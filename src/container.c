#include <container.h>
#include <routines/storage.h>
#include <routines/dbg.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>


void init_container(container_t *container, const char *name)
{
	char path[CONTAINER_MAX_NAME_LEN + sizeof(CONTAINER_PATH_STRING) + 8]; // 8 == len("/lib.so")

	snprintf(path, sizeof(path), "%s/lib%s.so", CONTAINER_PATH_STRING, name);

	container->name = strdup(name);
	if (!container->name)
		dbg_mem_exit();

	container->dlhandle = dlopen(path, RTLD_NOW); // TODO LAZY?
	if (!container->dlhandle)
		dbg_exit("Can't open container: %s\n", dlerror());
}

void free_container(container_t *container)
{
	free(container->name);
	dlclose(container->dlhandle);
}
