#include <inc/fs.h>
#include <inc/string.h>
#include <common.h>

FCB file[NR_FILES]; 
dir root;
char map[SECTSIZE];
ListHead file_head;

char buf[4096];
uint32_t index1[128];
uint32_t index2[128];

void testfs() {
//	ide_read(, buf, 1);
	int i;
	printk("testfs\n");
//	for(i = 0; i < DIR_FILES; ++ i) {
//		printk("%s\n", root.entry[i].filename);
//	}
//	return;
	int fd = fopen("1.txt", READ);	
	fread(fd, buf, 600);
	for(i = 0; i < 10; ++ i) printk("%d %c\n", i, buf[i]);
	fd = fopen("2.txt", WRITE);
	fseek(fd, 5, SEEK_SET);
	fwrite(fd, buf, 5);
	fd = fopen("2.txt", READ);
	fread(fd, buf, 26);
	for(i = 0; i < 26; ++ i) printk("%c\n", buf[i]);
	while(1);	
}

void init_fs() {
	ide_read(map, 0, 1);
	ide_read(&root, 1, 1);
	int i;
	list_init(&file_head);
	for(i = 3; i < NR_FILES; ++ i) {
		file[i].fd = i;
		file[i].bufno = -1;
		list_add_before(&file_head, &file[i].list);
	}
}


int fopen(const char *pathname, int flag) {
	int i;
	for(i = 0; i < DIR_FILES; ++ i) if(strcmp(pathname, root.entry[i].filename) == 0) break;
	printk("%d %s\n", i, pathname);
	assert(!list_empty(&file_head));
	FCB * fp = list_entry(file_head.next, FCB, list);
	list_del(&fp->list);
	fp->flag = flag;
	fp->offset = 0;
	ide_read(&fp->ino, root.entry[i].inode, 1);
//	fp->inode = root.entry[i].inode;
	return fp->fd;
}

int fread(int fd, void *buf, size_t len){
	if(file[fd].flag != READ) return -1;
//	inode tmp;
//	ide_read(&tmp, file[fd].inode, 1);
	while(len) {
		int i = file[fd].offset / SECTSIZE;
		if(i != file[fd].bufno) {
			if(i < 12) {
				ide_read(file[fd].buf, file[fd].ino.index[i], 1);
			} else {
				if(i < 12 + 128) {
					ide_read(index1, file[fd].ino.index[12], 1);
					ide_read(file[fd].buf, index1[i - 12], 1);
				} else {
					ide_read(index2, file[fd].ino.index[13], 1);
					ide_read(index1, index2[(i - 12 - 128) / 128], 1);
					ide_read(file[fd].buf, index1[i - 12 - 128], 1);
				}
			}
			file[fd].bufno = i;
		}
		int l = SECTSIZE - (file[fd].offset % SECTSIZE);
		if(l > len) l = len;
		memcpy(buf, file[fd].buf, l);
		file[fd].offset += l;
		buf += l;
		len -= l;
	}
	return len;
}


int fwrite(int fd, void *buf, size_t len) {
	if(file[fd].flag != WRITE) return -1;
//	inode tmp;
//	ide_read(&tmp, file[fd].inode, 1);
	while(len) {
		int i = file[fd].offset / SECTSIZE;
		if(i != file[fd].bufno) {
			ide_read(file[fd].buf, file[fd].ino.index[i], 1);
			file[fd].bufno = i;
		}
		int l = SECTSIZE - (file[fd].offset % SECTSIZE);
		if(l > len) l = len;
		memcpy(file[fd].buf + (file[fd].offset % SECTSIZE), buf, l);
		ide_write(file[fd].buf, file[fd].ino.index[i], 1);
		file[fd].offset += l;
		buf += l;
		len -= l;
	}
	return len;

}

int fseek(int fd, int offset, int whence) {
	switch (whence) {
		case SEEK_SET : file[fd].offset = offset; break;
		case SEEK_CUR : file[fd].offset += offset; break;
//		case SEEK_END : file[fd].offset = file_table[fd].size + offset; break;
		default : assert(0);
	}
	return file[fd].offset;
}

int fclose(int fd) {
	file[fd].flag = CLOSE;
	file[fd].bufno = -1;
	list_add_before(&file_head, &file[fd].list);
	return 0;
}

