#ifndef _SDR_SERVER_MODULE_H_
#define _SDR_SERVER_MODULE_H_


#include <semaphore.h>


#define MODULE_MAX_NAME_LEN	64

typedef struct instance instance_t;

typedef int (module_init_t)(instance_t *);
typedef void (module_handler_t)(instance_t *, void *data, size_t offset, size_t size);
typedef void (module_clean_t)(instance_t *);

typedef struct module {
	const char *description;
	module_init_t *init;
	module_handler_t *handler;
	module_clean_t *clean;
	const char **settings_fileds;
} module_t;

struct instance {
	char *name;
	module_t *module;
	void *context;

	instance_t *input;
	instance_t *output;
};


#define CONCATENATE_(x, y) x ## _ ## y
#define CONCATENATE(x, y) CONCATENATE_(x, y)
#define MODULE_PREFIX CONCATENATE(PROJECT_NAME, module)
#define MODULE_VARIABLE(name) CONCATENATE(MODULE_PREFIX, name)

#define STR_(x) #x
#define STR(x) STR_(x)
#define MODULE_PREFIX_STRING STR(MODULE_PREFIX)

#define ADD_MODULE(name_, description_, init_, handler_, clean_, settings_fileds_) \
	module_t MODULE_VARIABLE(name_) = {description_, init_, handler_, clean_, settings_fileds_}


extern module_t *get_module(void *dlhandle, const char *name);
extern int init_instance(instance_t *instance, const char *name);
extern void free_instance(instance_t *instance);
extern void set_output_instance_data(instance_t *instance, void *data, size_t offset, size_t size);
extern void begin_instance_loop(instance_t *instance);


#endif
