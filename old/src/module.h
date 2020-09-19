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

#ifndef _SDR_SERVER_MODULE_H_
#define _SDR_SERVER_MODULE_H_


#include "ring.h"


#define INIT_MODULE(module)	static void __attribute__((constructor)) module_init__() { module_init_(&module); }

// #define RING(size)	size
// #define DIRECT()	0xffff
	

typedef void(module_init_func_t)();
typedef int (module_init_instance_func_t)(int flags);
typedef int (module_close_func_t)();

typedef struct {
	const char *name;
	module_init_func_t *init;
	module_init_instance_func_t *instance_init;
	module_close_func_t *close;
	worker_func_t worker;
} module_t;

typedef struct module_instance {
	const char *name;
	module_t *module;
	struct module_instance *children[8];
	int children_count;
	void *private_data;
	ring_t *ring;
} module_instance_t;


extern void _init_module(module_t *module);


#endif
