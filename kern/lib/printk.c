#include <inc/types.h>
#include <inc/stdio.h>
#include <inc/stdarg.h>

void serial_putc(int);

static void putch(int ch, int *cnt)
{
	serial_putc(ch);
	cnt ++;
}

int printk(const char *fmt, ...) {
	va_list ap;
	int cnt = 0;

	va_start(ap, fmt);
	vprintfmt((void*)putch, &cnt, fmt, ap);
	va_end(ap);

	return cnt;
}

