#ifndef _SDR_SERVER_OPTION_H_
#define _SDR_SERVER_OPTION_H_


#include <stddef.h>


typedef enum {
	OPTION_TYPE_UNKNOWN = -1,
	OPTION_TYPE_STRING = 0,
	OPTION_TYPE_CHAR,
	OPTION_TYPE_UCHAR,
	OPTION_TYPE_INT,
	OPTION_TYPE_UINT,
	OPTION_TYPE_INT16,
	OPTION_TYPE_UINT16,
	OPTION_TYPE_INT32,
	OPTION_TYPE_UINT32,
	OPTION_TYPE_INT64,
	OPTION_TYPE_UINT64,
	OPTION_TYPE_FLOAT,
	OPTION_TYPE_DOUBLE,
	OPTION_TYPE_MAX
} option_type_t;

typedef struct {
	char *name;
	option_type_t type;
	size_t offset;
} option_t;


#define make_option(_name, _option_type, _struct, _member) \
	{ \
		.name = _name, \
		.type = _option_type, \
		.offset = offsetof(_struct, _member) \
	}

#define end_option() \
	{ \
		.name = NULL, \
		.type = OPTION_TYPE_UNKNOWN, \
		.offset = 0 \
	}


extern int set_option(void *context, option_t *option, const char *value);


#endif
