#ifndef LIB_THREAD_H
#define LIB_THREAD_H

#include <syscall.h>

static inline
void thread_create(int *x, void *entry, int *args) {
	syscall(SYS_thread_create, x, entry, args);
}

static inline
int sem_open(int cnt, bool bin) {
	syscall(SYS_sem_open, cnt, bin);
}

void sem_close(int id) {
	syscall(SYS_sem_close, id);
}

void sem_wait(int id) {
	syscall(SYS_sem_wait, id);
}

void sem_post(int id) {
	syscall(SYS_sem_post, id);
}

#endif
