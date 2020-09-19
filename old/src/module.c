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


#include "module.h"
#include <stdio.h>


#define MODULES_MAX_COUNT	16


static module_t *modules[MODULES_MAX_COUNT];

static int modules_count = 0;


void module_init_(module_t *module)
{
	if (modules_count >= MODULES_MAX_COUNT) {
		printf("Too many modules (>%d)\n", MODULES_MAX_COUNT);
		return;
	}
	modules[modules_count] = module;
	modules_count++;
	printf("Load '%s'\n", module->name);
}

int module_add_child()
{
}
