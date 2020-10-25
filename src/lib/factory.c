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
			dbg_assert_not_error_int(set_instance_module(instance, iterator->module_name));

		if (prev_instance) {
			instance->input = prev_instance;
			prev_instance->output = instance;
		}

		prev_instance = (iterator->terminator == CONNECTION_TERMINATOR) ? instance : NULL;
	}

	module_instance_foreach(instance) {
		printf("%s -> %s -> %s\n",
			   (instance->input) ? instance->input->name : "NULL",
			   instance->name,
			   (instance->output) ? instance->output->name : "NULL"
  			);
	}

	free_storage();
	free_config(&config);

	return 0;
}
