#include <module.h>
#include <option.h>

#include <stdlib.h>
#include <stddef.h>
#include <errno.h>


typedef struct {
	int option1;
	int option2;
} context_t;

option_t option[] = {
	make_option("option1", OPTION_TYPE_INT, context_t, option1),
	make_option("option2", OPTION_TYPE_INT, context_t, option2),
	end_option()
};


static int init(instance_t *instance)
{
	context_t *context;

	context = malloc(sizeof(context_t));
	if (context == NULL)
		return -ENOMEM;

	context->option1 = 1;
	context->option2 = 2;

	instance->context = context;

	return 0;
}

static void handler(instance_t *instance, void *data, size_t offset, size_t size)
{
	/* some work */
	(void)instance;
}

static void clean(instance_t *instance)
{
	free(instance->context);
}

static void handler2(instance_t *instance, void *data, size_t offset, size_t size)
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
