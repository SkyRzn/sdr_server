#ifndef _SDR_SERVER_CONFIG_H_
#define _SDR_SERVER_CONFIG_H_


#include <module.h>


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
	terminator_t terminator;
	struct config_iterator *(*next)(void);
} config_iterator_t;


extern config_iterator_t *config_iterator(const char *path);


#define config_foreach(path, iterator) for ( \
	iterator = config_iterator(path); \
	iterator != NULL; \
	iterator = iterator->next())


#endif
