#ifndef _SDR_SERVER_MAIN_STORAGE_H_
#define _SDR_SERVER_MAIN_STORAGE_H_


#include <module.h>
#include <thread.h>
#include <routines/autoarray.h>


#define thread_foreach(thread_) autoarray_foreach(_thread_array(), thread_)
#define module_instance_foreach(instance_) autoarray_foreach(_instance_array(), instance_)

extern void init_storage(void);
extern void free_storage(void);
extern instance_t *get_instance(const char *name);
extern int set_instance_module_name(instance_t *instance, const char *module_name);
extern autoarray_t *_instance_array(void);
extern autoarray_t *_thread_array(void);
extern thread_t *new_thread(void);


#endif
