#ifndef LIB_FS_H
#define LIB_FS_H

#include <syscall.h>
#include <stdio.h>

enum {CLOSE, READ, WRITE};
enum {SEEK_SET, SEEK_CUR, SEEK_END};

static inline
int fopen(const char * name, int flag) {
	return syscall(SYS_fopen, name, flag);
}

static inline
void fread(int fd, void *buf, int len) {
	syscall(SYS_fread, fd, buf, len);
	printf("%s", (char *)buf);
}

static inline
void fwrite(int fd, void *buf, int len) {
	syscall(SYS_fwrite, fd, buf, len);
}

static inline
void fseek(int fd, int offset, int whence) {
	syscall(SYS_fseek, fd, offset, whence);
}

static inline
void fclose(int fd) {
	syscall(SYS_fclose, fd);
}

#endif
