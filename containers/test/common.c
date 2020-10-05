#include <module.h>

#include <stddef.h>


static int handler(void *data, int count, void *instance)
{
	(void)data;
	(void)count;
	(void)instance;

	return 0;
}


ADD_MODULE(common_test,
		   NULL,
		   handler,
		   NULL);

ADD_MODULE(test2,
		   NULL,
		   handler,
		   NULL);

