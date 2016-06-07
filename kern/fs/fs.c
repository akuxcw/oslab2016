#include <inc/fs.h>
#include <inc/string.h>
#include <common.h>

FCB file[NR_FILES]; 
conf_t conf;
dir_t root;
uint8_t map[20 * SECTSIZE];
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
	int i;
	ide_read(&conf, 0, 1);
	ide_read(&root, conf.disk_size / SECTSIZE / SECTSIZE / 8 + 1, 1);
	if(conf.disk_size > 160 * SECTSIZE * SECTSIZE)
		conf.disk_size = 160 * SECTSIZE * SECTSIZE;
	for(i = 1; i <= conf.disk_size / SECTSIZE / SECTSIZE / 8; ++ i)
		ide_read(&map[SECTSIZE * (i-1)], i, 1);
	init_blocks(&map, conf.disk_size);
	list_init(&file_head);
	for(i = 3; i < NR_FILES; ++ i) {
		file[i].fd = i;
		file[i].bufno = -1;
		list_add_before(&file_head, &file[i].list);
	}
}

inode tmp;
int find_file(dir_t *dir, char *pathname) {
	while(pathname[0] == '/') pathname ++;
	if(*pathname == '\0') return -2;
	char *p = pathname;
	while(*p != '\0' && *p != '/') p ++;
	int i;
	for(i = 0; i < DIR_FILES; ++ i) 
		if(strncmp(pathname, dir->entry[i].filename, p - pathname) == 0) break;
	if(i >= DIR_FILES) return -1;
	if(*p == '\0') {
		ide_read(&tmp, dir->entry[i].inode, 1);
		if(tmp.type != F_FILE) {
			return -2;
		} else 
		return dir->entry[i].inode; 
	}
	else {
		dir_t newdir;
		ide_read(&newdir, dir->entry[i].inode, 1);
		return find_file(&newdir, p + 1);
	}
}

int fopen(char *pathname, int flag) {
	int i = find_file(&root, pathname);
	if(i == -1) panic("No such file : %s\n", pathname);
	if(i == -2) panic("It is a dir : %s\n", pathname);
	
	assert(!list_empty(&file_head));
	FCB * fp = list_entry(file_head.next, FCB, list);
	list_del(&fp->list);
	fp->flag = flag;
	fp->offset = 0;
	ide_read(&fp->ino, i, 1);
	fp->ino_id = i;
	return fp->fd;
}

int find_block(int fd) {
	int i = file[fd].offset / SECTSIZE;
	if(i < 12) {
		if(file[fd].ino.index[i] == 0)
			file[fd].ino.index[i] = get_new_block();
		return file[fd].ino.index[i];
	} else {
		if(i < 12 + 128) {
			if(file[fd].ino.index[12] == 0)
				file[fd].ino.index[i] = get_new_block();
			ide_read(index1, file[fd].ino.index[12], 1);
			if(index1[i - 12] == 0) {
				index1[i - 12] = get_new_block();
				ide_write(index1, file[fd].ino.index[12], 1);
			}
			return index1[i - 12];
		} else {
			if(file[fd].ino.index[13] == 0)
				file[fd].ino.index[i] = get_new_block();
			ide_read(index2, file[fd].ino.index[13], 1);
			if(index2[(i - 12 - 128) / 128] == 0) {
				index2[(i - 12 - 128) / 128] = get_new_block();
				ide_write(index2, file[fd].ino.index[13], 1);
			}
			ide_read(index1, index2[(i - 12 - 128) / 128], 1);
			if(index1[(i - 12 - 128) % 128] == 0) {
				index1[(i - 12 - 128) % 128] = get_new_block();
				ide_write(index1, index2[(i - 12 - 128) / 128], 1);
			}
			return index1[(i - 12 - 128) % 128];
		}
	}
	return 0;
}

int fread(int fd, void *buf, size_t len){
	if(file[fd].flag != READ) return -1;
	memset(buf, 0, sizeof buf);
	while(len) {
		int i = find_block(fd);
		if(i != file[fd].bufno) {
			ide_read(file[fd].buf, i, 1);
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
//	printk("%d\n", fd);
	while(len) {
		int i = find_block(fd);
//'		printk("%d\n", i);
		if(i != file[fd].bufno) {
			ide_read(file[fd].buf, i, 1);
			file[fd].bufno = i;
		}
		int l = SECTSIZE - (file[fd].offset % SECTSIZE);
		if(l > len) l = len;
		memcpy(file[fd].buf + (file[fd].offset % SECTSIZE), buf, l);
		ide_write(file[fd].buf, i, 1);
		file[fd].offset += l;
		buf += l;
		len -= l;
	}
	ide_write(&file[fd].ino, file[fd].ino_id, 1);
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

