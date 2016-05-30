#ifndef KERN_INC_FS_H
#define KERN_INC_FS_H

#include <inc/types.h>
#include <inc/mmu.h>
#include <inc/list.h>

#define SECTSIZE 512
#define OFFSET_IN_DISK	1000*1024
#define MAXNAMELEN	128
#define MAXFILESIZE	512 * (12 + 128 + 128 * 128 + 128 * 128 * 128)
#define MAXDISKSIZE (64 * 1024 * 1024)
// File types
#define NR_FILES	64

enum {CLOSE, READ, WRITE};
enum {SEEK_SET, SEEK_CUR, SEEK_END};
enum {F_DIR = 101, F_FILE};

typedef struct {
	uint32_t disk_size;
	uint8_t empty[SECTSIZE - 1 * sizeof(uint32_t)];
} conf_t;

typedef struct {
	char filename[24];
	uint32_t filesz;
	uint32_t inode;
} dirent;

#define DIR_FILES	SECTSIZE / sizeof(dirent)

typedef struct {
	dirent entry[DIR_FILES];
} dir_t;

typedef struct {
//	char filename[24];
	uint32_t type;
	uint32_t size;
	uint32_t index[15];
	uint8_t empty[SECTSIZE - 17 * sizeof(uint32_t)];
} inode;

typedef struct {
	uint32_t fd;
	uint32_t offset;
	uint32_t flag;
	inode ino;
	uint8_t buf[512];
	uint32_t bufno;
	ListHead list;
} FCB;

void	ide_read(void *, int, int);
void	ide_write(void *, int, int);

//int	file_create(const char *path, struct File **f);
void init_fs();
void testfs();
int	fopen(char *path, int flag);
int fread(int, void *, size_t);
int	fwrite(int, void *, size_t);
int fseek(int, int, int);
int fclose(int);

void init_blocks();
uint32_t get_new_block();
void del_block(uint32_t);
#endif
