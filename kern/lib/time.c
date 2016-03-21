#include <inc/syscall.h>

int time() {
	return syscall(SYS_time);
}

void sleep(int tmp) {
	syscall(SYS_sleep, tmp);
}
