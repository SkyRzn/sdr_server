/* Part of SDR server.
 *
 * Copyright (c) 2019 Alexandr Ivanov <alexandr.sky@gmail.com>
 *
 * This file is free software: you may copy, redistribute and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "modules.h"
#include "module.h"

#include "test/source.h"
#include "test/handler.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>


#define MAX_MODULES	16


static module_t *modules[] = {
	&module_test_source,
	&module_test_handler,
	NULL
};

static module_t *loaded[MAX_MODULES];
static int loaded_cnt = 0;

static int module_load(module_t *module, ring_t *source_ring);


ring_t *modules_load(const char *names[])
{
	const char **name;
	module_t **module;
	ring_t *ring = NULL;

	loaded_cnt = 0;

	for (name = names; *name; name++) {
		for (module = modules; *module; module++) {
			if (strcmp((*module)->name, *name) == 0) {
				if (loaded_cnt >= MAX_MODULES) {
					printf("Too many modules (max=%d)\n", MAX_MODULES);
					modules_unload();
					return NULL;
				}
				if (module_load(*module, ring) != 0) {
					modules_unload();
					return NULL;
				}
				ring = &(*module)->ring;
				loaded[loaded_cnt] = *module;
				loaded_cnt++;
			}
		}
	}
	return ring;
}

void modules_unload()
{
	int i;

	for (i = 0; i < loaded_cnt; i++) {
		loaded[i]->close();
		ring_free(&loaded[i]->ring);
	}
	loaded_cnt = 0;
}

static int module_load(module_t *module, ring_t *source_ring)
{
	int res;

	printf("Load module '%s' ring=%p, source_ring=%p\n", module->name, &module->ring, source_ring);

	module->source_ring = source_ring;

	module->init();

	if (!module->worker) {
		printf("Worker is NULL\n");
		return -EINVAL;
	}

	module->ring.worker = module->worker;

	res = ring_init(&module->ring);
	if (res != 0) {
		printf("Ring init error (module %s)\n", module->name);
		return res;
	}

	if (source_ring) {
		res = ring_add_reader(source_ring);
		if (res < 0) {
			ring_free(&module->ring);
			printf("Reader adding error (module %s)\n", module->name);
			return res;
		}
		module->reader_index = res;
	}

	return 0;
}
