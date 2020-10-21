#include <config.h>


CHEAT_TEST(config,
	config_t config;
	config_iterator_t *iterator;
	const char *instance_names[] = {"instance1", "instance2"};
	const char *module_names[] = {"container1:module1", "container2:module2"};
	int i;

	cheat_assert(load_config(NULL, "path") == -EINVAL);
	cheat_assert(load_config(&config, NULL) == -EINVAL);
	cheat_assert(load_config(&config, "/nonexistent_config_file_path") == -EINVAL);

	free_config(NULL);
	dbg_cheat_assert_error(first_config_iterator(NULL));

	cheat_assert(load_config(&config, "./test/config/test1.cfg") == 0);

	i = 0;
	config_foreach(&config, iterator) {
		cheat_assert(iterator != NULL);
		cheat_assert(iterator->pointer != NULL);
		cheat_assert(iterator->name != NULL);
		cheat_assert(strcmp(iterator->name, instance_names[i]) == 0);
		cheat_assert(iterator->module_name != NULL);
		cheat_assert(strcmp(iterator->module_name, module_names[i]) == 0);
		i++;
	}

	free_config(&config);
)
