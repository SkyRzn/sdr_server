#include <routines/dbg.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>


static int _errno = 0;


void __dbg(const char *file, const char *func, int line, const char *fmt, ...)
{
	va_list args;
	const char *p;

	p = strrchr(file, '/');
	if (p)
		file = p + 1;

	fprintf(stderr, "%s - %s[%d]: ", file, func, line);

	va_start (args, fmt);
	vfprintf(stderr, fmt, args);
	va_end (args);
}

void dbg_set_errno(int errno)
{
	_errno = errno;
}

int dbg_errno(void)
{
	return _errno;
}
