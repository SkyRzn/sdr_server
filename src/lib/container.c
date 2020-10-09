#include <container.h>
#include <routines/dbg.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>


int load_container(container_t *container, const char *name)
{
	void *dlhandle;
	char *namedup;
	char path[CONTAINER_MAX_NAME_LEN + sizeof(CONTAINER_PATH_STRING) + 8]; // 8 == len("/lib.so")

	if (container == NULL || name == NULL)
		dbg_return(-EINVAL, "Incorrect arguments");

	snprintf(path, sizeof(path), "%s/lib%s.so", CONTAINER_PATH_STRING, name);

	dlhandle = dlopen(path, RTLD_LAZY);
	if (dlhandle == NULL)
		dbg_return(-EACCES, "Can't open container: %s\n", dlerror());

	namedup = strdup(name);
	if (namedup == NULL) {
		dlclose(dlhandle);
		dbg_mem_return(-ENOMEM);
	}

	container->dlhandle = dlhandle;
	container->name = namedup;

	return 0;
}

void free_container(container_t *container)
{
	free(container->name);
	dlclose(container->dlhandle);
}
