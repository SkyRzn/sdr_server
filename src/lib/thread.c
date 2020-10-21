#include <thread.h>


int init_thread(thread_t *thread, module_instance_t *instance, thread_handler_t handler)
{
	int res;

	thread->instance = instance;

	res = pthread_create(&thread->pthread, NULL, handler, instance);
	if (res != 0)
		return res;

	return 0;
}

