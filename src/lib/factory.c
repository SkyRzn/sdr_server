#include <storage.h>
#include <config.h>
#include <routines/dbg.h>

#include <errno.h>
#include <stdio.h> // TEST


int init_factory(const char *config_path)
{
	config_iterator_t *iterator;
	instance_t *instance, *prev_instance = NULL;
	config_t config;

	dbg_assert_not_null(config_path, -EINVAL);

	dbg_assert_not_error_int(load_config(&config, config_path));

	init_storage();

	config_foreach(&config, iterator) {
		dbg_assert_not_null(iterator->name, -EINVAL);

		instance = get_instance(iterator->name);

		dbg_assert_not_null(instance, -ENOMEM);

		if (iterator->module_name)
			dbg_assert_not_error_int(set_instance_module_name(instance, iterator->module_name));

		if (iterator->options)
			instance->options_string = iterator->options;

		if (prev_instance) {
			instance->module->set_input(instance, prev_instance);
			prev_instance->module->set_output(prev_instance, instance);
		}

		prev_instance = (iterator->terminator == CONNECTION_TERMINATOR) ? instance : NULL;
	}

	module_instance_foreach(instance) {
		if (instance->module->init)
			instance->module->init(instance);

		instance_t *neighbour;
		autoarray_foreach(&instance->input, neighbour)
			printf("[%s] -> ", neighbour->name);

		printf("%s", instance->name);

		autoarray_foreach(&instance->output, neighbour)
			printf(" -> [%s]", neighbour->name);

		printf("\n");
	}

	module_instance_foreach(instance) {
		if (instance->module->free)
			instance->module->free(instance);
	}

	free_storage();
	free_config(&config);

	return 0;
}
