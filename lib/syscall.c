#include <inc/types.h>
#include <sys/syscall.h>
#include <sys/stat.h>

int __attribute__((__noinline__))
syscall(int id, ...) {
	int ret;
	int *args = &id;
//	Log("%x %x %x %x", args[0], args[1], args[2], args[3]);	
	asm volatile("int $0x80": "=a"(ret) : "a"(args[0]), "b"(args[1]), "c"(args[2]), "d"(args[3]));
	return ret;
}

void _exit(int status) {
//	nemu_assert(!status);
}

int open(const char *pathname, int flags) {
	return syscall(SYS_open, pathname, flags);
	return 0; 
}

int read(int fd, char *buf, int len) {
	return syscall(SYS_read, fd, buf, len);
	return 0; 
}

int write(int fd, char *buf, int len) {
	return syscall(SYS_write, fd, buf, len); 
}

off_t lseek(int fd, off_t offset, int whence) {
	return syscall(SYS_lseek, fd, offset, whence);
	return 0; 
}

void *sbrk(int incr) {
	extern char end;		/* Defined by the linker */
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0) {
		heap_end = &end;
	}
	prev_heap_end = heap_end;
	heap_end += incr;

	return prev_heap_end;
}

int close(int fd) {
	return syscall(SYS_close, fd);
	return 0; 
}

int fstat(int fd, struct stat *buf) {
	buf->st_mode = S_IFCHR;
	return 0;
}

int isatty(int fd) {
	return 0;
}
