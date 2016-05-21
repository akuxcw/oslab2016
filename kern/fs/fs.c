#include <inc/fs.h>
#include <inc/string.h>
#include <common.h>

enum {SEEK_SET, SEEK_CUR, SEEK_END};

FCB file[NR_FILES]; 
dir root;
char map[SECTSIZE];
ListHead file_head;

char buf[4096];

void testfs() {
//	ide_read(, buf, 1);
	int i;
	printk("testfs\n");
	for(i = 0; i < DIR_FILES; ++ i) {
		printk("%s\n", root.entry[i].filename);
	}
	return;
	int fd = fopen("1.txt", READ);	
	fread(fd, buf, 600);
//	ide_read(buf, 10, 3*SECTSIZE);
	for(i = 0; i < 600; ++ i) printk("%d %c\n", i, buf[i]);
	fd = fopen("2.txt", READ);	
	fread(fd, buf, 5);
//	ide_read(buf, 10, 3*SECTSIZE);
	for(i = 0; i < 5; ++ i) printk("%c\n", buf[i]);


}

void init_fs() {
	ide_read(map, 0, 1);
	ide_read(&root, 1, 1);
	int i;
	list_init(&file_head);
	for(i = 3; i < NR_FILES; ++ i) {
		file[i].fd = i;
		list_add_before(&file_head, &file[i].list);
	}
}


int fopen(const char *pathname, int flag) {
	int i;
	for(i = 0; i < DIR_FILES; ++ i) if(strcmp(pathname, root.entry[i].filename) == 0) break;
	assert(!list_empty(&file_head));
	FCB * fp = list_entry(file_head.next, FCB, list);
	list_del(&fp->list);
	fp->flag = flag;
	fp->offset = 0;
	fp->node = root.entry[i].inode_off;
	return fp->fd;
}

int fread(int fd, void *buf, size_t len){
	if(file[fd].flag != READ) return -1;
//	printk("read\n");
	inode tmp;
	ide_read(&tmp, SECTSIZE, file[fd].node);
//	printk("%d\n", tmp.index[0]);
	while(len) {
		int i = file[fd].offset / SECTSIZE;
		int offset = tmp.index[i] * SECTSIZE + (file[fd].offset % SECTSIZE);
//		printk("%x\n", offset);
		int l = SECTSIZE - (file[fd].offset % SECTSIZE);
		if(l > len) l = len;
		ide_read(buf, l, offset);
		file[fd].offset += l;
		buf += l;
		len -= l;
	}
	return len;
}

/*
int fwrite(int fd, void *buf, int len) {
	if(!FD[fd].opened) return -1;
	assert(FD[fd].offset + len < file_table[fd-3].size);
	ide_write(buf, FD[fd].offset, len);
	FD[fd].offset += len;
	return len;
}

int fseek(int fd, int offset, int whence) {
	switch (whence) {
		case SEEK_SET : FD[fd].offset = offset; break;
		case SEEK_CUR : FD[fd].offset += offset; break;
		case SEEK_END : FD[fd].offset = file_table[fd-3].size + offset; break;
		default : assert(0);
	}
	return FD[fd].offset;
}

int fclose(int fd) {
	FD[fd].opened = false;
	return 0;
}

*/
