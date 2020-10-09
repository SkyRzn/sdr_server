#include <storage.h>

#include <string.h>


CHEAT_TEST(storage,
	module_instance_t *instance;
	int res;

	init_storage();

	// check empty storage free
	free_storage();

	init_storage();

	instance = get_module_instance("test");
	(void)instance;
	cheat_assert(instance != NULL);

	res = set_instance_module(instance, "nonexistent_container:module");
	cheat_assert(res != 0);

	res = set_instance_module(instance, "common_test:common_test");
	cheat_assert(res == 0);

	res = set_instance_module(instance, "common_test");
	cheat_assert(res == 0);

	res = set_instance_module(instance, "common_test:test2");
	cheat_assert(res == 0);

	res = set_instance_module(instance, "common_test:nonexistent_module");
	cheat_assert(res != 0);

	free_storage();
)

