#ifndef _SDR_SERVER_MAIN_STORAGE_H_
#define _SDR_SERVER_MAIN_STORAGE_H_


#include <module.h>


extern void init_main_storage(void);
extern void free_main_storage(void);
extern module_instance_t *get_module_instance(const char *name);
extern void set_instance_module(module_instance_t *instance, const char *name);


#endif
