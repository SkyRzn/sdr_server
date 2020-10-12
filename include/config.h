#ifndef _SDR_SERVER_CONFIG_H_
#define _SDR_SERVER_CONFIG_H_


#include <module.h>

#include <stdbool.h>


typedef enum { // TODO move to connector.c?
	ZERO_TERMINATOR = -1,
	DIRECT_CONNECTION_TERMINATOR = 0,
	MULTITHREADED_CONNECTION_TERMINATOR,
	SEMICOLON_TERMINATOR,
	MAX_TERMINATOR
} terminator_t;

typedef struct config_iterator {
	const char *name;
	const char *module_name;
	char *settings;
	terminator_t terminator;
	struct config_iterator *(*next)(void);
} config_iterator_t;

typedef struct settings_iterator {
	const char *key;
	const char *value;
	struct settings_iterator *(*next)(void);
} settings_iterator_t;


extern config_iterator_t *first_config_iterator(const char *path);
extern settings_iterator_t *first_settings_iterator(char *settings);
extern bool check_config_end(config_iterator_t *iterator);


#define config_foreach(path, iterator) for ( \
	iterator = first_config_iterator(path); \
	check_config_end(iterator); \
	iterator = iterator->next())

#define settings_foreach(settings, iterator) for ( \
	iterator = first_settings_iterator(settings); \
	iterator != NULL; \
	iterator = iterator->next())


#endif
