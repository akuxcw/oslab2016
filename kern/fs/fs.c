#include <inc/fs.h>
#include <inc/string.h>
#include <common.h>

enum {SEEK_SET, SEEK_CUR, SEEK_END};

FCB file[NR_FILES]; 
dir root;
//bitmap map;
ListHead file_head;

void init_fs() {
//	ide_read(&map, SECTSIZE, 0);
	ide_read(&root, SECTSIZE, SECTSIZE);
	int i;
	list_init(&file_head);
	for(i = 3; i < NR_FILES; ++ i) {
		file[i].fd = i;
		list_add_before(&file_head, &file[i].list);
	}
}


int fopen(const char *pathname, int flag) {
	int i;
	for(i = 0; i < NR_FILES; ++ i) if(strcmp(pathname, root.entry[i].filename) == 0) break;
	FCB * fp = list_entry(&file_head.next, FCB, list);
	list_del(&fp->list);
	fp->flag = flag;
	fp->offset = 0;
	fp->node = root.entry[i].inode_off;
	return fp->fd;
}

int fread(int fd, void *buf, size_t len){
	if(file[fd].flag != READ) return -1;
	inode tmp;
	ide_read(&tmp, SECTSIZE, file[fd].node);
	int i = len / SECTSIZE;
	int offset = tmp.index[i] + (len % SECTSIZE);
	ide_read(buf, len, offset);
	file[fd].offset += len;
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
