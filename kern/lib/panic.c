#include <inc/types.h>
#include <inc/stdio.h>
#include <inc/x86.h>

void serial_putc(int);

static void putch(int ch, int *cnt)
{
	serial_putc(ch);
}

void _panic(const char *file, int line, const char *fmt, ...) {
	va_list ap;
	int cnt = 0;
	asm volatile ("cli");	
	printk("{kernel panic at %s : %d}   ", file, line);
	va_start(ap, fmt);
	vprintfmt((void*)putch, &cnt, fmt, ap);
	va_end(ap);
	printk("\n");
	outw(0x8A00, 0x8A00);
	outw(0x8A00, 0x8E00);
	while(1);
}

