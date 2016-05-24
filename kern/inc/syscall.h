#ifndef __SYSCALL_H__
#define __SYSCALL_H__

enum {SYS_, SYS_write, SYS_palette, SYS_kbd, SYS_time, SYS_sleep, SYS_fork, SYS_exit,
	SYS_thread_create, SYS_thread_join, SYS_sem_open, SYS_sem_close, SYS_sem_wait, SYS_sem_post,
	SYS_fopen, SYS_fread, SYS_fwrite, SYS_fseek};

static inline int //__attribute__((__noinline__))
syscall(int id, ...) {
	int ret;
	int *args = &id;
	asm volatile("int $0x80": "=a"(ret) : "a"(args[0]), "b"(args[1]), "c"(args[2]), "d"(args[3]));
	return ret;
}

#endif
