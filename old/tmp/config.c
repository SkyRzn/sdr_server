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

#include "config.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int parse_config(const char *path)
{
	FILE *f;
	char *line = NULL;
	size_t len = 0;
	
	f = fopen(path, "r");
	if (!f)
		return -ENOENT;
	
	while (getline(&line, &len, f) != -1) {
		printf("line: %s", line);
	}

	free(line);
	fclose(f);	
	
	return 0;
}

int main()
{
	parse_config("../example.cfg");
	return 0;
}
