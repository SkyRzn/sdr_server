#include <module.h>
#include <routines/dbg.h>

#include <string.h>
#include <errno.h>


CHEAT_TEST(module,
	module_t *module;
	int res;

	dbg_set_errno(0);
	module = get_module("module", NULL);
	cheat_assert(module == NULL);
	cheat_assert(dbg_errno() == -EINVAL);

	dbg_set_errno(0);
	module = get_module(NULL, "name");
	cheat_assert(module == NULL);
	cheat_assert(dbg_errno() == -EINVAL);

	res = init_module_instance(NULL, NULL);
	cheat_assert(res == -EINVAL);
)
