#include <config.h>
#include <routines/dbg.h>


#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <fcntl.h>
#include <errno.h>


#define READ_BLOCK_SIZE	4096

#define is_space(c) (c <= 0x20 || c > 0x7e)


static config_iterator_t iterator = {NULL, NULL, ZERO_TERMINATOR, NULL};

static char *buf = NULL, *current = NULL, *next = NULL;

static const char *terminator_array[] = {
	[DIRECT_CONNECTION_TERMINATOR]        = "->",
	[MULTITHREADED_CONNECTION_TERMINATOR] = "=>",
	[SEMICOLON_TERMINATOR]                = ";",
	[MAX_TERMINATOR]                      = NULL};


static config_iterator_t *config_next_iterator(void);
static terminator_t find_terminator(void);
static char *read_file(const char *path);
static void clean_config(void);


config_iterator_t *config_iterator(const char *path)
{
	iterator.next = config_next_iterator;

	current = buf = read_file(path);

	clean_config();

	return config_next_iterator();
}

static config_iterator_t *config_next_iterator(void)
{
	if (!buf || !current || !iterator.next)
		return NULL;

	iterator.terminator = find_terminator();
	iterator.name = current;
	iterator.module_name = NULL; // TODO

	current = next;

	if (iterator.terminator == ZERO_TERMINATOR) {
		free(buf);
		buf = NULL;
	}

	return &iterator;
}

static terminator_t find_terminator(void)
{
	const char **terminator = terminator_array;
	int term_index, nearest_index = -1, min_distance = -1;

	for (term_index = 0; term_index < MAX_TERMINATOR; term_index++) {
		next = strstr(current, terminator[term_index]);

		if (!next)
			continue;

		if (min_distance == -1 || next - current < min_distance) {
			min_distance = next - current;
			nearest_index = term_index;
		}
	}

	if (!next)
		return ZERO_TERMINATOR;

	next = current + min_distance;

	if (*next != '\0') {
		*next = '\0';
		next += strlen(terminator[nearest_index]);
	}

	return nearest_index;
}

static char *read_file(const char *path)
{
	char *res = NULL;
	int fd, size = 0, cnt = 0;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		dbg_exit("Can't open config file %s\n", path);

	res = malloc(READ_BLOCK_SIZE);

	while ((size = read(fd, res, READ_BLOCK_SIZE)) >= 0) {
		if (size < READ_BLOCK_SIZE) {
			size += READ_BLOCK_SIZE * cnt;

			res = realloc(res, size + 1);
			if (!res)
				dbg_mem_exit();

			res[size] = '\0';

			close(fd);

			return res;
		}

		cnt++;

		res = realloc(res, READ_BLOCK_SIZE * (cnt + 1));
	}

	close(fd);

	dbg_exit("Config file reading error: %s\n", strerror(errno));

	return NULL;
}

static void clean_config(void)
{
	char *dst = NULL, *src;

	if (*buf == '\0')
		dbg_exit("Config is empty\n");

	for (src = buf; *src != '\0'; src++) {
		if (is_space(*src)) {
			if (!dst)
				dst = src;
			continue;
		}

		if (*src == '#') { // skip comments till '\n'
			if (!dst)
				dst = src;
			do {
				src++;
				if (*src == '\n') {
					src++;
					break;
				}
			} while (*src != '\0');

			if (is_space(*src) || *src == '#') {
				src--;
				continue;
			}
		}

		if (dst) {
			*dst = *src;
			dst++;
		}
	}
	*dst = '\0';
}
