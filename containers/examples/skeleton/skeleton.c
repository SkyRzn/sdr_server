#include <module.h>

#include <stdlib.h>
#include <stddef.h>
#include <errno.h>


typedef struct {
	int a;
	int b;
} context_t;


static int init(module_instance_t *instance)
{
	context_t *context;

	context = malloc(sizeof(context_t));
	if (context == NULL)
		return -ENOMEM;

	context->a = 1;
	context->b = 2;

	instance->context = context;

	return 0;
}

static void handler(module_instance_t *instance)
{
	/* some work */
	(void)instance;
}

static void clean(module_instance_t *instance)
{
	free(instance->context);
}

static void handler2(module_instance_t *instance)
{
	/* some work */
	(void)instance;
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


ADD_MODULE(skeleton,       // note: without quotes!
		   description,
		   init,
		   handler,
		   clean,
		   settings);

ADD_MODULE(skeleton2, NULL, NULL, handler2, NULL, NULL);
