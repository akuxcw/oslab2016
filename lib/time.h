#ifndef LIB_TIME_H
#define LIB_TIME_H
#include <syscall.h>
static inline
int time() {
	return syscall(SYS_time);
}
static inline
void sleep(int tmp) {
	return;
	syscall(SYS_sleep, tmp);
}
#endif
