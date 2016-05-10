#ifndef LIB_THREAD_H
#define LIB_THREAD_H

#include <syscall.h>
#define sem_t int
static inline
void thread_create(int *x, void *entry, int *args) {
	syscall(SYS_thread_create, x, entry, args);
}

static inline
int sem_open(char *name, int cnt, int bin) {
	return syscall(SYS_sem_open, name, cnt, bin);
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
