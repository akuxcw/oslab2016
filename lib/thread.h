#ifndef LIB_THREAD_H
#define LIB_THREAD_H

#include <syscall.h>

static inline
void thread_create(int *x, void *entry, int *args) {
	syscall(SYS_thread_create, x, entry, args);
}

#endif
