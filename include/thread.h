#ifndef _SDR_SERVER_THREAD_H_
#define _SDR_SERVER_THREAD_H_


#include <module.h>

#include <pthread.h>


typedef void *(thread_handler_t)(void *);

typedef struct {
	instance_t *instance;
	pthread_t pthread;
} thread_t;


extern int init_thread(thread_t *thread, instance_t *instance, thread_handler_t handler);


#endif
