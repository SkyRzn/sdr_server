#include <module.h>
#include <routines/dbg.h>

#include <string.h>
#include <errno.h>


CHEAT_TEST(module,
	module_t *module;
	int res;

	dbg_cheat_assert_error(module = get_module("module", NULL));
	cheat_assert(module == NULL);

	dbg_cheat_assert_error(module = get_module(NULL, "name"));
	cheat_assert(module == NULL);

	res = init_instance(NULL, NULL);
	cheat_assert(res == -EINVAL);
)
