#include <inc/types.h>
#include <inc/stdio.h>

void serial_putc(int);

static void putch(int ch, int *cnt)
{
	serial_putc(ch);
}

void _panic(const char *file, int line, const char *fmt, ...) {
	va_list ap;
	int cnt = 0;
	
	printk("[kernel panic at %s : %d]", file, line);
	va_start(ap, fmt);
	vprintfmt((void*)putch, &cnt, fmt, ap);
	va_end(ap);
	printk("\n");

}

