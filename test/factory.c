#include <factory.h>


CHEAT_TEST(factory,
	cheat_assert(run_factory(NULL) == -EINVAL);
)
