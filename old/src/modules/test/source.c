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


#include "../../module.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define RING_LENGTH	8
#define DATA_LENGTH	65536


static module_t module;


static void init()
{
// 	module.ring.length = RING_LENGTH;
// 	module.ring.data_length = DATA_LENGTH;
// 
// 	printf("test source inited\n");
}

static int instance_init(int flags)
{
// 	module.ring.length = RING_LENGTH;
// 	module.ring.data_length = DATA_LENGTH;
// 
// 	printf("test source inited\n");
	return 0;
}

static int close_()
{
	printf("test source close\n");
	return 0;
}

static void *worker(void *args)
{
// 	ring_t *ring = &module.ring;
// 
// 	do {
// 		unsigned char *data = (unsigned char *)ring->wr_block->data;
// 		printf(">>> %p == %p\n", data, ring->wr_block->data);
// 		for (int i = 0; i < DATA_LENGTH; i++) {
// 			data[i] = i % 256;
// 		}
// 	} while (ring_writer_push(ring) == 0);

	return NULL;
}


static module_t module = {
	.name = "test_source",
	.init = init,
	.instance_init = instance_init,
	.close = close_,
	.worker = worker
};

INIT_MODULE(module);
