#ifndef _SDR_SERVER_MODULE_H_
#define _SDR_SERVER_MODULE_H_


#include <routines/autoarray.h>

#include <semaphore.h>


#define MODULE_MAX_NAME_LEN	64

typedef struct module_instance module_instance_t;

typedef void (module_handler_t)(module_instance_t *);

typedef struct module {
	char *name;
	const char *description;
	module_handler_t *handler;
	const char **settings_fileds;
} module_t;

struct module_instance {
	char *name;
	module_t *module;
	void *data;
	size_t data_size;
	autoarray_t output_instance_array;
};


#define CONCATENATE_(x, y) x ## _ ## y
#define CONCATENATE(x, y) CONCATENATE_(x, y)
#define MODULE_PREFIX CONCATENATE(PROJECT_NAME, module)
#define MODULE_VARIABLE(name) CONCATENATE(MODULE_PREFIX, name)

#define STR_(x) #x
#define STR(x) STR_(x)
#define MODULE_PREFIX_STRING STR(MODULE_PREFIX)

#define ADD_MODULE(name_, description_, handler_, settings_fileds_) \
	module_t MODULE_VARIABLE(name_) = {STR(name_), description_, handler_, settings_fileds_}


extern module_t *get_module(void *dlhandle, const char *name);
extern int init_module_instance(module_instance_t *instance, const char *name);
extern void free_module_instance(module_instance_t *instance);


#endif
