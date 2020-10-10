#include <routines/autoarray.h>
#include <routines/dbg.h> // TEST


#include <string.h>


CHEAT_TEST(routines_autoarray,
	typedef struct {
		char *name;
		int val;
	} test_t;

	char buf[16];
	int i, res;
	test_t *test;
	autoarray_t test_array;
	autoarray_t test_named_array;

	res = init_autoarray(NULL, test_t);
	cheat_assert(res == -EINVAL);

	free_autoarray(NULL);

	init_autoarray(&test_array, test_t);
	init_named_autoarray(&test_named_array, test_t, name);

	test = new_autoarray_item(NULL);
	cheat_assert(test == NULL);

	test = get_autoarray_item_by_index(NULL, 0);
	cheat_assert(test == NULL);
	test = get_autoarray_item_by_index(&test_array, 0);
	cheat_assert(test == NULL);
	test = get_autoarray_item_by_index(&test_array, -1);
	cheat_assert(test == NULL);
	test = get_autoarray_item_by_index(&test_array, 1);
	cheat_assert(test == NULL);

	test = get_autoarray_item_by_name(NULL, "test");
	cheat_assert(test == NULL);
	test = get_autoarray_item_by_name(&test_array, NULL);
	cheat_assert(test == NULL);
	test = get_autoarray_item_by_name(&test_array, "test");
	cheat_assert(test == NULL);

	test = new_autoarray_item(&test_array);
	cheat_assert(test != NULL);

	test = get_autoarray_item_by_index(&test_array, 0);
	cheat_assert(test != NULL);
	test = get_autoarray_item_by_index(&test_array, 1);
	cheat_assert(test == NULL);

	test = get_autoarray_item_by_name(&test_array, "test");
	cheat_assert(test == NULL);

	free_autoarray(&test_array);

	for (i = 0; i < 100; i++) {
		snprintf(buf, sizeof(buf), "name%d", i);

		test = new_autoarray_item(&test_array);
		cheat_assert(test != NULL);
		test->val = i;

		test = new_autoarray_item(&test_named_array);
		cheat_assert(test != NULL);
		test->name = strdup(buf);
	}

	test = get_autoarray_item_by_name(&test_named_array, "name10");
	cheat_assert(test != NULL);
	cheat_assert(strcmp(test->name, "name10") == 0);

	test = get_autoarray_item_by_name(&test_array, "name10");
	cheat_assert(test == NULL);

	i = 0;
	autoarray_foreach(&test_array, test)
		cheat_assert(test->val == i++);

	i = 0;
	autoarray_foreach(&test_named_array, test) {
		snprintf(buf, sizeof(buf), "name%d", i++);
		cheat_assert(strcmp(test->name, buf) == 0);
		free(test->name);
	}

	free_autoarray(&test_named_array);
	free_autoarray(&test_array);
)
