#ifndef KERN_INC_FS_H
#define KERN_INC_FS_H

#include <inc/types.h>
#include <inc/mmu.h>

#define SECTSIZE 512
// File nodes (both in-memory and on-disk)

// Bytes per file system block - same as page size
#define BLKSIZE		PGSIZE
#define BLKBITSIZE	(BLKSIZE * 8)

// Maximum size of a filename (a single path component), including null
// Must be a multiple of 4
#define MAXNAMELEN	128

// Maximum size of a complete pathname, including null
#define MAXPATHLEN	1024

// Number of block pointers in a File descriptor
#define NDIRECT		10
// Number of direct block pointers in an indirect block
#define NINDIRECT	(BLKSIZE / 4)

#define MAXFILESIZE	((NDIRECT + NINDIRECT) * BLKSIZE)

// An inode block contains exactly BLKFILES 'struct File's
#define BLKFILES	(BLKSIZE / sizeof(struct File))

// File types
#define FTYPE_REG	0	// Regular file
#define FTYPE_DIR	1	// Directory


// File system super-block (both in-memory and on-disk)


// Definitions for requests from clients to file system
enum {
	FSREQ_OPEN = 1,
	FSREQ_SET_SIZE,
	// Read returns a Fsret_read on the request page
	FSREQ_READ,
	FSREQ_WRITE,
	// Stat returns a Fsret_stat on the request page
	FSREQ_STAT,
	FSREQ_FLUSH,
	FSREQ_REMOVE,
	FSREQ_SYNC
};

bool	ide_probe_disk1(void);
void	ide_set_disk(int diskno);
void	ide_set_partition(uint32_t first_sect, uint32_t nsect);
int	ide_read(uint32_t secno, void *dst, size_t nsecs);
int	ide_write(uint32_t secno, const void *src, size_t nsecs);

//int	file_create(const char *path, struct File **f);
int	fopen(const char *path, int flag);
int fread(int fd, void *buf, size_t count);
int	fwrite(int fd, const void *buf, size_t count);

#endif
