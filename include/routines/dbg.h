#ifndef _SDR_SERVER_ROUTINES_DBG_H_
#define _SDR_SERVER_ROUTINES_DBG_H_


#include <unistd.h>
#include <stdlib.h>


#define dbg(fmt, ...) dbg_(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define dbg_exit(fmt, ...) \
	do { \
		dbg_(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
		exit(EXIT_FAILURE); \
	} while (0)
#define dbg_mem_exit() dbg_exit("Memory allocation error\n")

extern void dbg_(const char *file, const char *func, int line, const char *fmt, ...)
	__attribute__((format(printf, 4, 5)));


#endif
