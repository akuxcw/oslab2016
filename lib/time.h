#ifndef LIB_TIME_H
#define LIB_TIME_H
#include <syscall.h>

int time() {
	return syscall(SYS_time);
}

void sleep(int tmp) {
	syscall(SYS_sleep, tmp);
}
#endif
