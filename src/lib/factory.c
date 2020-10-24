#include <storage.h>
#include <config.h>
#include <routines/dbg.h>

#include <errno.h>
#include <stdio.h> // TEST


void init_factory(const char *config_path)
{
	config_iterator_t *iterator;
	instance_t *instance, *prev_instance = NULL;
	config_t config;

	dbg_assert_not_null(config_path, );

	if (load_config(&config, config_path) != 0)
		return;

	init_storage();

	config_foreach(&config, iterator) {
		dbg_assert_not_null(iterator->name, );

		instance = get_instance(iterator->name);

		dbg_assert_not_null(instance, );

		if (iterator->module_name)
			dbg_assert_not_error(set_instance_module(instance, iterator->module_name), );

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
}
