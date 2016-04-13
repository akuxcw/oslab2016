#include <inc/types.h>
#include <inc/stdio.h>
#include <inc/stdarg.h>

void serial_putc(int);

static void putch(int ch, int *cnt)
{
	serial_putc(ch);
	cnt ++;
}

int __printk(const char *fmt, ...) {
	va_list ap;
	int cnt = 0;

	va_start(ap, fmt);
	vprintfmt((void*)putch, &cnt, fmt, ap);
	va_end(ap);

	return cnt;
}

int _printk(const char * file, int line, const char *fmt, ...) {
	__printk("%s,%d {kernel} ", file, line);
	va_list ap;
	int cnt = 0;

	va_start(ap, fmt);
	vprintfmt((void*)putch, &cnt, fmt, ap);
	va_end(ap);

	return cnt;

}
