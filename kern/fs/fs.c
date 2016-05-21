#include <inc/fs.h>
#include <common.h>
/*
typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

static const file_info file_table[] = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

typedef struct {
	bool opened;
	uint32_t offset;
} Fstate;

Fstate FD[NR_FILES + 3]; 


int fopen(const char *pathname, int flags) {
	int i;
//	Log("%s", pathname);
	for(i = 0; i < NR_FILES; ++ i) if(strcmp(pathname, file_table[i].name) == 0) break;
	assert(i < NR_FILES);
	if(i < NR_FILES) {
		FD[i + 3].opened = true;
		FD[i + 3].offset = 0;//file_table[i].disk_offset;
		return i + 3;
	}
	return -1;
}

int fread(int fd, void *buf, int len){
	if(!FD[fd].opened) return -1;
//	assert(FD[fd].offset + len <= file_table[fd-3].size);
	int remain = file_table[fd-3].size - FD[fd].offset;
	if(remain < len) len = remain;
	ide_read(buf, file_table[fd-3].disk_offset + FD[fd].offset, len);
	FD[fd].offset += len;
	return len;
	if(strlen(buf) == 0) return -1; else return strlen(buf);
}

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
