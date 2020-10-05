#ifndef _SDR_SERVER_MAIN_STORAGE_H_
#define _SDR_SERVER_MAIN_STORAGE_H_


#include <module.h>


extern void init_storage(void);
extern void free_storage(void);
extern module_instance_t *get_module_instance(const char *name);
extern int set_instance_module(module_instance_t *instance, const char *module_name);


#endif
