#ifndef INC_STDIO_H
#define INC_STDIO_H

#include <inc/stdarg.h>

#ifndef NULL
#define NULL	((void *) 0)
#endif /* !NULL */

// lib/printfmt.c
void vprintfmt(void (*putch)(int, void*), void *putdat, const char *fmt, va_list);

int vsnprintf(char *, int, const char *, va_list);
int snprintf(char *, int, const char *, ...);

// lib/printf.c
#define printk(...) _printk(__FILE__, __LINE__, __VA_ARGS__)
int	_printk(const char *, int, const char *fmt, ...);
int	__printk(const char *fmt, ...);

#endif 
