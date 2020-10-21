#ifndef _SDR_SERVER_CONFIG_H_
#define _SDR_SERVER_CONFIG_H_


#include <module.h>

#include <regex.h>
#include <stdbool.h>


typedef enum {
	ZERO_TERMINATOR = -1,
	CONNECTION_TERMINATOR = 0,
	SEMICOLON_TERMINATOR,
	MAX_TERMINATOR
} terminator_t;

typedef struct {
	char *pointer;
	char *name;
	char *module_name;
	char *settings;
	terminator_t terminator;
} config_iterator_t;

typedef struct {
	char *buffer;
	regex_t regex;
} config_t;


#define config_foreach(_config, _iterator) for ( \
	init_config_iterator(_config, _iterator); \
	(_iterator)->pointer != NULL; \
	push_config_iterator(_config, _iterator))
#define settings_foreach(_settings, _iterator) for ( \
	_iterator = first_settings_iterator(_settings); \
	_iterator != NULL; \
	_iterator = (_iterator)->next())


extern int load_config(config_t *config, const char *path);
extern void free_config(config_t *config);
extern void init_config_iterator(config_t *config, config_iterator_t *iterator);
extern void push_config_iterator(config_t *config, config_iterator_t *iterator);

#endif
