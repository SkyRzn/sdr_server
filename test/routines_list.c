#include <routines/list.h>

#include <string.h>


CHEAT_TEST(routines_list,
	char buf[16], *name;
	int i, res;

	struct test {
		struct test *next;
		const char *name;
		int val;
	} *test;

	list_t test_list;
	list_t test_named_list;

	init_list(NULL, struct test, next);
	free_list(NULL);

	init_list(&test_list, struct test, next);
	init_named_list(&test_named_list, struct test, next, name);

	res = add_list_item(NULL, NULL);
	cheat_assert(res != 0);

	res = add_list_item(&test_list, NULL);
	cheat_assert(res != 0);

	for (i = 0; i < 100; i++) {
		snprintf(buf, sizeof(buf), "name%d", i);

		test = malloc(sizeof(test));
		test->name = strdup(buf);

		res = add_list_item(&test_list, test);
		cheat_assert(res == 0);
		test->val = i;

		test = malloc(sizeof(test));
		test->name = strdup(buf);

		res = add_list_item(&test_named_list, test);
		cheat_assert(res == 0);
	}

	test = find_list_item(&test_named_list, "name10");
	cheat_assert(test != NULL);
	cheat_assert(strcmp(test->name, "name10") == 0);

	test = find_list_item(&test_list, "name10");
	cheat_assert(test == NULL);

	i = 0;
	list_foreach(&test_list, test)
		cheat_assert(test->val == i++);

	i = 0;
	named_list_foreach(&test_named_list, test, name) {
		snprintf(buf, sizeof(buf), "name%d", i++);
		cheat_assert(name != NULL);
		cheat_assert(strcmp(test->name, name) == 0);
		cheat_assert(strcmp(buf, name) == 0);
		free(name);
	}

	free_list(&test_named_list);
	free_list(&test_list);
)
