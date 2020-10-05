#include <module.h>

#include <string.h>


CHEAT_TEST(module,
	module_t *module;

	module = get_module("module", NULL);
	cheat_assert(module == NULL);

	module = get_module(NULL, NULL);
	cheat_assert(module == NULL);
)
