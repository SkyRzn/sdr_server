#include <module.h>

#include <string.h>
#include <errno.h>


CHEAT_TEST(module,
	module_t *module;
	int res;

	module = get_module("module", NULL);
	cheat_assert(module == NULL);

	module = get_module(NULL, NULL);
	cheat_assert(module == NULL);

	res = init_module_instance(NULL, NULL);
	cheat_assert(res == -EINVAL);
)
