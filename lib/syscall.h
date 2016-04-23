#ifndef LIB_SYSCALL_H
#define LIB_SYSCALL_H

enum {SYS_, SYS_write, SYS_palette, SYS_kbd, SYS_time, SYS_sleep, SYS_fork, SYS_exit};

static inline int //__attribute__((__noinline__))
syscall(int id, ...) {
	int ret;
	int *args = &id;
	asm volatile("int $0x80": "=a"(ret) : "a"(args[0]), "b"(args[1]), "c"(args[2]), "d"(args[3]));
	return ret;
}

#endif
