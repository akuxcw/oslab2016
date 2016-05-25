#ifndef KERN_INC_FS_H
#define KERN_INC_FS_H

#define SECTSIZE 512

#define MAXFILESIZE	512 * (12 + 128 + 128 * 128 + 128 * 128 * 128)


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

enum {F_DIR = 101, F_FILE};

typedef struct {
	char filename[24];
	int filesz;
	int inode_off;
} dirent;

typedef struct {
	dirent entry[SECTSIZE / sizeof(dirent)];
} dir;

typedef struct {
	int type;
	int size;
	int index[15];
} inode;

#endif
