#ifndef LIB_PROCESS_H
#define LIB_PROCESS_H

#include <syscall.h>

static inline
int fork() {
	syscall(SYS_fork);
}

#endif
