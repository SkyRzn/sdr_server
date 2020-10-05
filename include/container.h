#ifndef _SDR_SERVER_CONTAINER_H_
#define _SDR_SERVER_CONTAINER_H_


#include <module.h>


#define CONTAINER_MAX_NAME_LEN	64
#define FULL_MAX_NAME_LEN	(MODULE_MAX_NAME_LEN + MODULE_MAX_NAME_LEN + 2) // + ':' + '\0'

#define STR_(x) #x
#define STR(x) STR_(x)
#define CONTAINER_PATH_STRING	STR(CONTAINER_PATH)


typedef struct container {
	char *name;
	void *dlhandle;
	struct container *next;
} container_t;


extern container_t *load_container(const char *name);
extern void free_container(container_t *container);


#endif
