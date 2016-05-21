#include <inc/fs.h>
#include <inc/x86.h>
#include <inc/assert.h>
#include <common.h>

static inline
void waitdisk(void) {
	while((inb(0x1F7) & 0xC0) != 0x40); 
}

static inline
void readsect(void *dst, int offset) {
	waitdisk();
	outb(0x1F2, 1);
	outb(0x1F3, offset);
	outb(0x1F4, offset >> 8);
	outb(0x1F5, offset >> 16);
	outb(0x1F6, (offset >> 24) | 0xE0);
	outb(0x1F7, 0x20);

	waitdisk();
	insl(0x1F0, dst, SECTSIZE/4);
}

static inline
void writesect(void *dst, int offset) {
	waitdisk();
	outb(0x1F2, 1);
	outb(0x1F3, offset);
	outb(0x1F4, offset >> 8);
	outb(0x1F5, offset >> 16);
	outb(0x1F6, (offset >> 24) | 0xE0);
	outb(0x1F7, 0x30);

	waitdisk();
	outsl(0x1F0, dst, SECTSIZE/4);
}

void ide_read(void *buf, int secno, int nsecs) {
	secno += OFFSET_IN_DISK / SECTSIZE;
	for(; nsecs ; secno ++, nsecs --, buf += SECTSIZE)
		readsect(buf, secno);
}

void ide_write(void *buf, int secno, int nsecs) {
	secno += OFFSET_IN_DISK / SECTSIZE;
	for(; nsecs ; secno ++, nsecs --, buf += SECTSIZE)
		writesect(buf, secno);
}

