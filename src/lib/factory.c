#include <storage.h>
#include <config.h>
#include <routines/dbg.h>


int run_factory(const char *config_path)
{
	if (!config_path || !*config_path)
		dbg_return(-1, "Config path is empty!"); //TODO add error code?

	config_iterator_t *iterator;

	config_foreach(config_path, iterator) {
		if (!iterator->module_name || !iterator->name)
			continue;

		init_storage();

		module_instance_t *instance = get_module_instance(iterator->module_name);

		if (!instance) 
			continue;

		if (set_instance_module(instance, iterator->name) == 0) {

		}

		free_storage();	
	}

	return 0;
}
