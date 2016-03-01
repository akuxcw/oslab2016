// Simple implementation of printk console output for the kernel,
// based on printfmt() and the kernel console's cputchar().

#include <inc/types.h>
#include "inc/stdio.h"
#include "inc/stdarg.h"

static void
putch(int ch, int *cnt)
{
	cputchar(ch);
//	*cnt++;
}

int
vcprintk(const char *fmt, va_list ap)
{
	int cnt = 0;

	vprintfmt((void*)putch, &cnt, fmt, ap);
	return cnt;
}

int
printk(const char *fmt, ...)
{
	va_list ap;
	int cnt;

	va_start(ap, fmt);
	cnt = vcprintk(fmt, ap);
	va_end(ap);

	return cnt;
}

