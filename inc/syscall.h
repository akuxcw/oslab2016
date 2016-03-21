#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define SYS_write 1 
#define SYS_palette 2
#define SYS_kbd 3

static inline int //__attribute__((__noinline__))
syscall(int id, ...) {
	int ret;
	int *args = &id;
	asm volatile("int $0x80": "=a"(ret) : "a"(args[0]), "b"(args[1]), "c"(args[2]), "d"(args[3]));
	return ret;
}

#endif
