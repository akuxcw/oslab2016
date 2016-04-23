#ifndef LIB_PROCESS_H
#define LIB_PROCESS_H

#include <syscall.h>

static inline
int fork() {
	return syscall(SYS_fork);
}

static inline
void exit(int x) {
	syscall(SYS_exit, x);
}

#endif
