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

#ifndef _SDR_SERVER_RING_H_
#define _SDR_SERVER_RING_H_


#include <pthread.h>
#include <semaphore.h>


#define RING_MAX_READERS	8

#define RING_FLAG_STOP		0x01

#define RING_IS_STOPPED		-1000


typedef enum {
	RING_STATE_CREATED = 0,
	RING_STATE_MEM_ALLOCATED,
	RING_STATE_CTRL_MTX_INITED,
	RING_STATE_WR_SEM_INITED,
	RING_STATE_RD_SEM_INITED,
	RING_STATE_THREAD_CREATED
} ring_state_t;

typedef struct ring_block_t ring_block_t;
struct ring_block_t {
	int readers_count;
	void *data;
	ring_block_t *next;
};

typedef void *(*worker_func_t)(void *);

typedef struct {
	int length;
	int data_length;
	int readers_count;
	int flags;

	ring_state_t state;

	pthread_t thread;
	pthread_mutex_t ctrl_mtx;
	sem_t wr_sem;
	sem_t rd_sem;

	worker_func_t worker;

	ring_block_t *wr_block;
	ring_block_t *rd_blocks[RING_MAX_READERS];

	ring_block_t *blocks;
} ring_t;


extern int ring_init(ring_t *ring);
extern void ring_free(ring_t *ring);
extern int ring_add_reader(ring_t *ring);
extern int ring_writer_push(ring_t *ring);
int ring_reader_push(ring_t *ring, int reader_index);


#endif

