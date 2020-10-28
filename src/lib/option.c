#include <option.h>
#include <routines/dbg.h>

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>


#define option_foreach(_settings, _iterator) for ( \
	_iterator = first_settings_iterator(_settings); \
	_iterator != NULL; \
	_iterator = (_iterator)->next())


int set_option(void *context, option_t *option, const char *value)
{
	void *member = ((void *)context + option->offset);
	int res = 0;

	switch (option->type) {
		case OPTION_TYPE_STRING:
			*(char **)member = strdup(value);
			if (*(char **)member == NULL)
				res = -ENOMEM;
			break;
		case OPTION_TYPE_CHAR:
			if (sscanf(value, "%" SCNi8, (char *)member) != 1)
				res= -EINVAL;
			break;
		case OPTION_TYPE_UCHAR:
			if (sscanf(value, "%" SCNu8, (unsigned char *)member) != 1)
				res= -EINVAL;
			break;
		case OPTION_TYPE_INT:
			if (sscanf(value, "%i", (int *)member) != 1)
				res= -EINVAL;
			break;
		case OPTION_TYPE_UINT:
			if (sscanf(value, "%u", (unsigned int *)member) != 1)
				res= -EINVAL;
			break;
		case OPTION_TYPE_INT16:
			if (sscanf(value, "%" SCNi16, (int16_t *)member) != 1)
				res= -EINVAL;
			break;
		case OPTION_TYPE_UINT16:
			if (sscanf(value, "%" SCNu16, (uint16_t *)member) != 1)
				res= -EINVAL;
			break;
		case OPTION_TYPE_INT32:
			if (sscanf(value, "%" SCNi32, (int32_t *)member) != 1)
				res= -EINVAL;
			break;
		case OPTION_TYPE_UINT32:
			if (sscanf(value, "%" SCNu32, (uint32_t *)member) != 1)
				res= -EINVAL;
			break;
		case OPTION_TYPE_INT64:
			if (sscanf(value, "%" SCNi64, (int64_t *)member) != 1)
				res= -EINVAL;
			break;
		case OPTION_TYPE_UINT64:
			if (sscanf(value, "%" SCNu64, (uint64_t *)member) != 1)
				res= -EINVAL;
			break;
		case OPTION_TYPE_FLOAT:
			if (sscanf(value, "%f", (float *)member) != 1)
				res= -EINVAL;
			break;
		case OPTION_TYPE_DOUBLE:
			if (sscanf(value, "%lf", (double *)member) != 1)
				res= -EINVAL;
			break;
		default:
			dbg("Incorrect option type: %d\n", option->type);
			return -EINVAL;
	}

	return res;
}

void free_option(void *context, option_t *option)
{
	if (option->type == OPTION_TYPE_STRING)
		free(*(char **)((void *)context + option->offset));
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
