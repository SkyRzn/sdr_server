#include <module.h>

#include <stddef.h>


static void handler(module_instance_t *instance)
{
	(void)instance;
}


ADD_MODULE(common_test,
		   NULL,
		   handler,
		   NULL);

ADD_MODULE(test2,
		   NULL,
		   handler,
		   NULL);

