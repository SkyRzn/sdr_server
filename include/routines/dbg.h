#ifndef _SDR_SERVER_ROUTINES_DBG_H_
#define _SDR_SERVER_ROUTINES_DBG_H_


#include <unistd.h>
#include <stdlib.h>


extern int _dbg_errno;


#ifdef TEST_COVERAGE
#  define _dbg(_fmt, ...) do { } while (0)
#else
#  define _dbg(_fmt, ...) __dbg(__FILE__, __FUNCTION__, __LINE__, _fmt, ##__VA_ARGS__)
#endif

#  define dbg(_fmt, ...) _dbg(_fmt, ##__VA_ARGS__)

#define dbg_return(_value, _fmt, ...) \
	do { \
		_dbg(_fmt, ##__VA_ARGS__); \
		return _value; \
	} while (0)
#define dbg_exit(_fmt, ...) \
	do { \
		_dbg(_fmt, ##__VA_ARGS__); \
		exit(EXIT_FAILURE); \
	} while (0)
#define dbg_mem() dbg("Memory allocation error\n")
#define dbg_mem_return(_value) \
	do { \
		dbg("Memory allocation error\n"); \
		return _value; \
	} while (0)
#define dbg_mem_exit() dbg_exit("Memory allocation error\n") // TODO deprecated

#ifdef TEST_COVERAGE
#define dbg_assert(_condition, _retval, _fmt, ...) \
	do { \
		if (!(_condition)) { \
			_dbg(_fmt, ##__VA_ARGS__); \
			dbg_set_errno(-1); \
			return _retval; \
		} \
	} while (0)
#else
#define dbg_assert(_condition, _retval, _fmt, ...) \
	do { \
		if (!(_condition)) { \
			_dbg(_fmt, ##__VA_ARGS__); \
			exit(-1); \
		} \
	} while (0)
#endif

#define dbg_malloc_assert(_pointer, _retval) \
	dbg_assert(_pointer != NULL, _retval, "Memory allocation error\n")

#define dbg_assert_not_null(_pointer, _retval) \
	dbg_assert(_pointer != NULL, _retval, "Pointer '" #_pointer "' is NULL\n")

#define dbg_assert_not_error(_value, _retval) \
	do { \
		int __value = _value; \
		dbg_assert(__value == 0, _retval, "Error %d (" #_value ")\n", __value); \
	} while(0)

#define dbg_assert_not_error_int(_value) \
	do { \
		int __retval = _value; \
		(void) __retval; \
		dbg_assert_not_error(_value, __retval); \
	} while(0)

#define dbg_cheat_assert_error(_function) \
	dbg_set_errno(0); \
	_function; \
	cheat_assert(dbg_errno() != 0)


extern void __dbg(const char *file, const char *func, int line, const char *fmt, ...)
	__attribute__((format(printf, 4, 5)));
extern void dbg_set_errno(int errno);
extern int dbg_errno(void);


#endif
