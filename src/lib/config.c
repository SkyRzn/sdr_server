#include <config.h>
#include <routines/dbg.h>


#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <fcntl.h>
#include <errno.h>


#define READ_BLOCK_SIZE	4096
#define ELEMENT_PATTERN "(<[[:alnum:]_:]+>)*([[:alnum:]_]+)(\\([[:alnum:]_,.=]+\\))*"

#define is_space(c) (c <= 0x20 || c > 0x7e)


static char *buf = NULL, *current = NULL, *next = NULL, *settings_buf = NULL, *settings_current = NULL;

static const char *terminator_array[] = {
	[DIRECT_CONNECTION_TERMINATOR]        = "->",
	[MULTITHREADED_CONNECTION_TERMINATOR] = "=>",
	[SEMICOLON_TERMINATOR]                = ";",
	[MAX_TERMINATOR]                      = NULL};

static regex_t regex;


static config_iterator_t *next_config_iterator(void);
static settings_iterator_t *next_settings_iterator(void);
static config_iterator_t *parse_current_element(void);
static terminator_t find_terminator(void);
static char *read_file(const char *path);
static void free_iterator(void);
static void clean_config(void);


static config_iterator_t iterator = {NULL, NULL, NULL, ZERO_TERMINATOR, next_config_iterator};

static settings_iterator_t settings_iterator = {NULL, NULL, next_settings_iterator};


config_iterator_t *first_config_iterator(const char *path)
{
	current = buf = read_file(path);

	clean_config();

	if (regcomp(&regex, ELEMENT_PATTERN, REG_EXTENDED) != 0)
		dbg_exit("Regcomp error\n");

	return next_config_iterator();
}

settings_iterator_t *first_settings_iterator(char *settings)
{
	settings_current  = settings_buf = settings;

	return next_settings_iterator();
}

static config_iterator_t *next_config_iterator(void)
{
	config_iterator_t *iter;

	if (!buf || !current)
		return NULL;

	iterator.name = NULL;
	iterator.module_name = NULL;
	iterator.settings = NULL;
	iterator.terminator = find_terminator();

	iter = parse_current_element();

	current = next;

	if (iterator.terminator == ZERO_TERMINATOR)
		free_iterator();

	return iter;
}

static settings_iterator_t *next_settings_iterator(void)
{
	if (!settings_buf || !settings_current)
		return NULL;

	settings_iterator.key = settings_current;

	settings_current = strchr(settings_current, '=');
	if (!settings_current)
		dbg_exit("Incorrect settings field: '%s'\n", settings_buf);

	*settings_current = '\0';

	settings_iterator.value = ++settings_current;

	settings_current = strchr(settings_iterator.value, ',');
	if (settings_current) {
		*settings_current = '\0';
		settings_current++;
	}

	return &settings_iterator;
}

static config_iterator_t *parse_current_element(void)
{
	regmatch_t pmatch[4];
	char *beg, *end;

	if (!current || *current == '\0')
		return NULL;

	if (regexec(&regex, current, 4, pmatch, 0) != 0)
		dbg_exit("Can't parse config element: '%s'\n", current);

	for (int i = 1; i < 4; i++) { // skip full match
		if (pmatch[i].rm_so < 0 || pmatch[i].rm_eo <= 0)
			continue;

		beg = current + pmatch[i].rm_so;
		end = current + pmatch[i].rm_eo;

		if (beg[0] == '<') { // module name
			*(end - 1) = '\0';
			iterator.module_name = beg + 1;
		} else if (beg[0] == '(' || *(end - 1) == ')') { // settings
			*(end - 1) = '\0';
			iterator.settings = beg + 1;
		} else { // instance name
			*end = '\0';
			iterator.name = beg;
		}
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

static void free_iterator(void)
{
	if (!buf)
		return;

	free(buf);
	regfree(&regex);
	buf = NULL;
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

