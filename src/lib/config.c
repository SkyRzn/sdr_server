#include <config.h>
#include <routines/dbg.h>


#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>


#define READ_BLOCK_SIZE	4096
#define ELEMENT_PATTERN "(<[[:alnum:]_:]+>)*([[:alnum:]_]+)(\\([[:alnum:]_,.=]+\\))*"

#define is_space(c) (c <= 0x20 || c > 0x7e)


static const char *terminator_array[] = {
	[CONNECTION_TERMINATOR] = "->",
	[SEMICOLON_TERMINATOR]  = ";",
	[MAX_TERMINATOR]        = NULL};


static int parse_current_element(config_t *config, config_iterator_t *iterator);
static terminator_t find_terminator(char *current, char **next);
static char *read_file(const char *path);
static void clean_config(char *buffer);


int load_config(config_t *config, const char *path)
{
	dbg_assert_not_null(path, -EINVAL);
	dbg_assert_not_null(config, -EINVAL);

	dbg_assert_not_error_int(regcomp(&config->regex, ELEMENT_PATTERN, REG_EXTENDED));

	config->buffer = read_file(path);
	if (config->buffer == NULL)
		return -EINVAL;

	clean_config(config->buffer);

	return 0;
}

void free_config(config_t *config)
{
	dbg_assert_not_null(config, );

	free(config->buffer);
	regfree(&config->regex);
	free(config);
}

void init_config_iterator(config_t *config, config_iterator_t *iterator)
{
	iterator->pointer = config->buffer;
	iterator->name = NULL;
	iterator->module_name = NULL;
	iterator->settings = NULL;
	iterator->terminator = SEMICOLON_TERMINATOR;

	push_config_iterator(config, iterator);
}

void push_config_iterator(config_t *config, config_iterator_t *iterator)
{
	char *next_pointer;

	iterator->terminator = find_terminator(iterator->pointer, &next_pointer);

	if (parse_current_element(config, iterator) == 0)
		iterator->pointer = next_pointer;
}

static int parse_current_element(config_t *config, config_iterator_t *iterator)
{
	regmatch_t pmatch[4];
	char *beg, *end;

	if (!iterator->pointer || *iterator->pointer == '\0')
		return -EINVAL;

	if (regexec(&config->regex, iterator->pointer, 4, pmatch, 0) != 0)
		dbg_exit("Can't parse config element: '%s'\n", iterator->pointer);

	for (int i = 1; i < 4; i++) { // skip full match
		if (pmatch[i].rm_so < 0 || pmatch[i].rm_eo <= 0)
			continue;

		beg = iterator->pointer + pmatch[i].rm_so;
		end = iterator->pointer + pmatch[i].rm_eo;

		if (beg[0] == '<') { // module name
			*(end - 1) = '\0';
			iterator->module_name = beg + 1;
		} else if (beg[0] == '(' || *(end - 1) == ')') { // settings
			*(end - 1) = '\0';
			iterator->settings = beg + 1;
		} else { // instance name
			*end = '\0';
			iterator->name = beg;
		}
	}

	return 0;
}

static terminator_t find_terminator(char *current, char **next)
{
	const char **terminator = terminator_array;
	int term_index, nearest_index = -1, min_distance = -1;

	for (term_index = 0; term_index < MAX_TERMINATOR; term_index++) {
		*next = strstr(current, terminator[term_index]);

		if (!*next)
			continue;

		if (min_distance == -1 || *next - current < min_distance) {
			min_distance = *next - current;
			nearest_index = term_index;
		}
	}

	if (!*next)
		return ZERO_TERMINATOR;

	*next = current + min_distance;

	if (**next != '\0') {
		**next = '\0';
		*next += strlen(terminator[nearest_index]);
	}

	return nearest_index;
}

static char *read_file(const char *path)
{
	char *res = NULL;
	int fd, size = 0, cnt = 0;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		dbg_return(NULL, "Can't open config file %s\n", path);

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

static void clean_config(char *buffer)
{
	char *dst = NULL, *src;

	if (*buffer == '\0')
		dbg_exit("Config is empty\n");

	for (src = buffer; *src != '\0'; src++) {
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

// static settings_iterator_t *next_settings_iterator(void)
// {
// 	if (!settings_buf || !settings_current)
// 		return NULL;
//
// 	settings_iterator.key = settings_current;
//
// 	settings_current = strchr(settings_current, '=');
// 	if (!settings_current)
// 		dbg_exit("Incorrect settings field: '%s'\n", settings_buf);
//
// 	*settings_current = '\0';
//
// 	settings_iterator.value = ++settings_current;
//
// 	settings_current = strchr(settings_iterator.value, ',');
// 	if (settings_current) {
// 		*settings_current = '\0';
// 		settings_current++;
// 	}
//
// 	return &settings_iterator;
// }
