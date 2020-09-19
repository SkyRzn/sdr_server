#include <stdlib.h>
#include <stdio.h>


void test_start_block(const char *name)
{
	printf("==== test %s\n", name);
}

void test_assert(int result, const char *message)
{
	if (!result) {
		printf("\tERROR: %s\n", message);
		exit(-1);
	}
}

