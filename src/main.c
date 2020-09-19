#include <main_storage.h>

#include <stdio.h>


int main(int argc, const char **argv)
{
	init_main_storage();

	module_instance_t *instance = get_module_instance("skeleton");
	set_instance_module(instance, "skeleton");

	printf("%s - %s\n", instance->module->name, instance->name);

	free_main_storage();
}
