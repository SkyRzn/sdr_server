#include <config.h>


CHEAT_TEST(config,
	config_iterator_t *iterator;
	const char *instance_names[] = {"instance1", "instance2"};
	const char *module_names[] = {"container1:module1", "container2:module2"};
	int i;

	dbg("!!!!!!!!!!! 0\n");
	iterator = first_config_iterator(NULL);
	cheat_assert(iterator == NULL);
	dbg("!!!!!!!!!!! 1\n");

	iterator = first_config_iterator("/nonexistent_config_file_path");
	cheat_assert(iterator == NULL);
	dbg("!!!!!!!!!!! 2\n");

	i = 0;
	config_foreach("./test/config/config1.cfg", iterator) {
		cheat_assert(iterator != NULL);
		cheat_assert(iterator->name != NULL);
		cheat_assert(strcmp(iterator->name, instance_names[i]) == 0);
		cheat_assert(iterator->module_name != NULL);
		dbg("'%s'=='%s'\n", iterator->module_name, module_names[i]);
		cheat_assert(strcmp(iterator->module_name, module_names[i]) == 0);

		i++;
	}
)
