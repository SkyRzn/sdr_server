#include <storage.h>

#include <string.h>


CHEAT_TEST(storage,
	instance_t *instance;
	int res;

	init_storage();

	// check empty storage free
	free_storage();

	init_storage();


	instance = get_instance("test");
	cheat_assert(instance != NULL);

	cheat_assert(set_instance_module_name(instance, NULL) == -EINVAL);
	cheat_assert(set_instance_module_name(NULL, "name") == -EINVAL);

	res = set_instance_module_name(instance, "nonexistent_container:module");
	cheat_assert(res != 0);

	res = set_instance_module_name(instance, "common_test:source");
	cheat_assert(res == 0);

	res = set_instance_module_name(instance, "common_test:doubling");
	cheat_assert(res == 0);

	res = set_instance_module_name(instance, "common_test:nonexistent_module");
	cheat_assert(res != 0);

	free_storage();
)

