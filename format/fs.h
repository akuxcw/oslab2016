#ifndef KERN_INC_FS_H
#define KERN_INC_FS_H

#define SECTSIZE 512

#define MAXNAMELEN	128

// File types
#define FTYPE_REG	0	// Regular file
#define FTYPE_DIR	1	// Directory

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

typedef struct {
	char filename[24];
	int filesz;
	int inode_off;
} dirent;

typedef struct {
	dirent entry[SECTSIZE / sizeof(dirent)];
} dir;

typedef struct {
	int index[512 / sizeof(int)];
} inode;

#endif
