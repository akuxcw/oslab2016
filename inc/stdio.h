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
int	printk(const char *fmt, ...);

int	printf(const char *fmt, ...);

#endif 
