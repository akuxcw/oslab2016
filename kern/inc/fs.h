#ifndef KERN_INC_FS_H
#define KERN_INC_FS_H

#include <inc/types.h>
#include <inc/mmu.h>
#include <inc/list.h>

#define SECTSIZE 512
#define OFFSET_IN_DISK	1000*1024
#define MAXNAMELEN	128

// File types
#define FTYPE_REG	0	// Regular file
#define FTYPE_DIR	1	// Directory
#define NR_FILES	64
enum {CLOSE, READ, WRITE};

typedef struct {
	char filename[24];
	uint32_t filesz;
	uint32_t inode;
} dirent;

#define DIR_FILES	SECTSIZE / sizeof(dirent)

typedef struct {
	dirent entry[DIR_FILES];
} dir;

typedef struct {
	uint32_t index[512 / sizeof(uint32_t)];
} inode;

typedef struct {
	uint32_t fd;
	uint32_t offset;
	uint32_t flag;
	uint32_t inode;
	uint8_t buf[512];
	uint32_t bufno;
	ListHead list;
} FCB;

void	ide_read(void *, int, int);
void	ide_write(void *, int, int);

//int	file_create(const char *path, struct File **f);
void init_fs();
void testfs();
int	fopen(const char *path, int flag);
int fread(int, void *, size_t);
int	fwrite(int, void *, size_t);

#endif
