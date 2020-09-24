#include <module.h>

#include <stddef.h>


static int handler(void *data, int count, void *instance)
{
	module_instance_t *inst = (module_instance_t *)instance;
	/* some work */
	(void)inst;

	return 0;
}

static int handler_plus5(void *data, int count, void *instance)
{
	module_instance_t *inst = (module_instance_t *)inst;
	/* some work */
	(void)instance;

	return 0;
}


static const char *settings[] = {
	"field1",
	"field2"
};

#define description "Lorem ipsum dolor sit amet, consectetur adipiscing elit. " \
	"Integer enim nunc, vehicula vel molestie sodales, eleifend vel metus. " \
	"Aenean blandit tincidunt velit id condimentum. Sed quis ipsum turpis. " \
	"Maecenas nec erat vel nunc hendrerit elementum blandit ut magna. " \
	"Integer tincidunt dolor in elit dapibus, ut volutpat enim commodo. " \
	"Pellentesque dapibus nisl dictum justo mattis vestibulum. Duis vel vestibulum augue. " \
	"Maecenas sit amet lectus justo."


ADD_MODULE(skeleton,       // note: without quites!
		   description,
		   handler,
		   settings);

ADD_MODULE(plus5, NULL, handler_plus5, NULL);
