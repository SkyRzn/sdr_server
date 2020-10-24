#include <factory.h>


CHEAT_TEST(factory,
	cheat_assert(init_factory(NULL) == -EINVAL);
)
