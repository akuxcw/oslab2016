#include <inc/stdio.h>
#include <inc/syscall.h>

int printf(const char * fmt, ...) {
	static char buf[256];
	void *args = (void **)&fmt + 1;
	int len = vsnprintf(buf, 256, fmt, args);
	
	return syscall(SYS_write, 0, buf, len);
}
