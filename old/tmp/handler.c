/* Part of dump740, a UVD protocol decoder for RTLSDR devices.
 *
 * Copyright (c) 2016 Alexandr Ivanov <alexandr.sky@gmail.com>
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


#include "handler.h"
#include <stdio.h>


#define RING_LENGTH	8
#define DATA_LENGTH	65536


static void test_handler_init()
{
	module_test_handler.ring.length = RING_LENGTH;
	module_test_handler.ring.data_length = DATA_LENGTH;
}

static int test_handler_close()
{
	return 0;
}

static void *test_handler_worker(void *args)
{
	ring_t *ring = module_test_handler.source_ring;

	while (ring_reader_push(ring, module_test_handler.reader_index) == 0) {
		unsigned char *data = (unsigned char *)ring->rd_blocks[module_test_handler.reader_index]->data;
		for (int i = 0; i < 40; i++) {
			printf("%02x ", data[i]);
		}
		printf("\n");
	}

	return NULL;
}


module_t module_test_handler = {
	.name = "test_handler",
	.init = test_handler_init,
	.close = test_handler_close,
	.worker = test_handler_worker
};
