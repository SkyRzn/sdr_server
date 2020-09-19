#include <module.h>

#include <stddef.h>


int handler(char *data, int count, void *instance)
{
	module_instance_t *inst = (module_instance_t *)instance;
	/* some work */
	(void)inst;

	return 0;
}

int handler_plus5(char *data, int count, void *instance)
{
	module_instance_t *inst = (module_instance_t *)inst;
	/* some work */
	(void)instance;

	return 0;
}


static module_t module = {
	.name = "skeleton",
	.description = "description",
	.handler = handler,
};
ADD_MODULE(module, skeleton);

static module_t module_plus5 = {
	.name = "plus5",
	.handler = handler_plus5,
};
ADD_MODULE(module_plus5, plus5);
