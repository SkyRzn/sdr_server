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


#include "ring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


int ring_init(ring_t *ring)
{
	int i;

	ring->state = RING_STATE_CREATED;
	ring->readers_count = 0;
	ring->flags = 0;

	if (ring->length < 2) {
		printf("Length should be 2 or more\n");
		return -EINVAL;
	}

	if (!ring->worker) {
		printf("Worker is NULL\n");
		return -EINVAL;
	}

	ring->blocks = malloc(sizeof(ring_block_t) * ring->length);
	if (!ring->blocks)
		return -ENOMEM;

	for (i = 0; i < ring->length; i++) {
		ring_block_t *block;

		block = &ring->blocks[i];
		block->data = malloc(ring->data_length);
		if (!block->data) {
			ring->length = i + 1;
			ring_free(ring);
			return -ENOMEM;
		}

		block->readers_count = 0;

		if (i > 0) {
			ring->blocks[i-1].next = block;
			if (i == ring->length-1)
				block->next = &ring->blocks[0];
		}
	}
	ring->state = RING_STATE_MEM_ALLOCATED;

	ring->wr_block = ring->blocks->next;

	if (pthread_mutex_init(&ring->ctrl_mtx, NULL) != 0) {
		printf("Control mutex error");
		goto error;
	}
	ring->state = RING_STATE_CTRL_MTX_INITED;

	if (sem_init(&ring->wr_sem, 0, 0) != 0) { // TODO setup sem value
		printf("Writer semaphore error");
		goto error;
	}
	ring->state = RING_STATE_WR_SEM_INITED;

	if (sem_init(&ring->rd_sem, 0, 0) != 0) { // TODO setup sem value
		printf("Reader semaphore error");
		goto error;
	}
	ring->state = RING_STATE_RD_SEM_INITED;

	if (pthread_create(&ring->thread, NULL, ring->worker, NULL) != 0) {
		printf("Writer semaphore error");
		goto error;
	}
	ring->state = RING_STATE_THREAD_CREATED;

	return 0;

error:
	ring_free(ring);
	return -EINVAL;
}

void ring_free(ring_t *ring)
{
	int i;

	switch (ring->state) {
		case RING_STATE_CTRL_MTX_INITED:
			pthread_mutex_destroy(&ring->ctrl_mtx);
		case RING_STATE_WR_SEM_INITED:
			sem_destroy(&ring->wr_sem);
		case RING_STATE_RD_SEM_INITED:
			sem_destroy(&ring->rd_sem);
		case RING_STATE_THREAD_CREATED:
		case RING_STATE_MEM_ALLOCATED:
			for (i = 0; i < ring->length; i++)
				free(ring->blocks[i].data);
			free(ring->blocks);
		default:
			;
	}
}

int ring_start(ring_t *ring)
{
	return 0;
}

int ring_add_reader(ring_t *ring)
{
	int i;

	pthread_mutex_lock(&ring->ctrl_mtx);

	i = ring->readers_count;
	if (i == RING_MAX_READERS)
		return -ENOMEM;

	ring->rd_blocks[i] = ring->blocks;
	ring->rd_blocks[i]->readers_count++;
	ring->readers_count++;

	pthread_mutex_unlock(&ring->ctrl_mtx);

	return i;
}

int ring_writer_push(ring_t *ring)
{
	int wait = 0, res = 0;
	ring_block_t *block = ring->wr_block;

	pthread_mutex_lock(&ring->ctrl_mtx);
	ring->wr_block = block->next;
	if (ring->flags & RING_FLAG_STOP)
		res = RING_IS_STOPPED;
	else
		wait = (ring->wr_block->readers_count > 0);

	for (int i = 0; i < block->readers_count; i++)
		sem_post(&ring->rd_sem);

	pthread_mutex_unlock(&ring->ctrl_mtx);

	if (wait)
		sem_wait(&ring->wr_sem);

	return res;
}

int ring_reader_push(ring_t *ring, int reader_index)
{
	ring_block_t *block;
	int wait = 0, res = 0;

	pthread_mutex_lock(&ring->ctrl_mtx);

	block = ring->rd_blocks[reader_index];

	ring->rd_blocks[reader_index] = block->next;
	block->readers_count--;
	if (block->readers_count == 0 && ring->wr_block == block)
		sem_post(&ring->wr_sem);

	if (ring->flags & RING_FLAG_STOP)
		res = RING_IS_STOPPED;
	else
		wait = (block->next == ring->wr_block);
	
	block->next->readers_count++;

	pthread_mutex_unlock(&ring->ctrl_mtx);

	if (wait)
		sem_wait(&ring->rd_sem);

	return res;
}
