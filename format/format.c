#include "fs.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

dir d[512];
int c[512];
char map[512];
int index0[MAXFILESIZE / SECTSIZE];
int index1[MAXFILESIZE / SECTSIZE];
int size, tot = 2, dir_tot = 0;
FILE *fout;
char buf[512];

int min(int a, int b) {
	return a < b ? a : b;
}
void create_file(char *pathname) {
	int j, nr_block;
	FILE *fin;
	memset(index0, 0, sizeof index0);
	memset(index1, 0, sizeof index1);

	fin = fopen(pathname, "rb");
	fseek(fin, 0, SEEK_END);
	int len = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	if(len > MAXFILESIZE) {
		printf("File %s is too large.", pathname);
		assert(0);
	}
	nr_block = (len - 1) / SECTSIZE + 1;
//		printf("%d\n", nr_block);
	for(j = 0; j < nr_block; ++ j) {
		index0[j] = tot + j + 1;
	}

	strcpy(d[dir_tot].entry[c[dir_tot]].filename, &pathname[5]);
	d[dir_tot].entry[c[dir_tot]].filesz = len;
	d[dir_tot].entry[c[dir_tot]].inode_off = tot;
	c[dir_tot] ++;

	inode tmp;
	memset(&tmp, 0, sizeof tmp);
	tmp.size = len; 

	for(j = 0; j < min(nr_block, 12); j ++) {
		tmp.index[j] = tot + j + 1;
	}
	if(nr_block > 12) {
		tmp.index[12] = tot + nr_block + 1;
		fseek(fout, tmp.index[12] * 512, SEEK_SET);
		fwrite(&index0[12], 1, 512, fout);
		if(nr_block > 12 + 128) {
			tmp.index[13] = tot + nr_block + 2;
			for(j = 0; j < min(128, (nr_block - 12 - 128 - 1) / 128); ++ j) {
				index1[j] = tot + nr_block + 2 + j + 1;
			}
			fseek(fout, tmp.index[13] * 512, SEEK_SET);
			fwrite(index1, 1, 512, fout);
			for(j = 0; j < min(128, (nr_block - 12 - 128 - 1) / 128 + 1); ++ j) {
				fseek(fout, (j + tmp.index[13] + 1) * 512, SEEK_SET);
				fwrite(&index0[12 + 128 + j * 128], 1, 512, fout);
			}
		}
	}

	//		printf("%d\n", tot);
	fseek(fout, tot * 512, SEEK_SET);
	tot ++;
	fwrite(&tmp, 1, 512, fout);

	for(j = 0; j < len; j += 512) {
		fread(buf, 1, 512, fin);
		fwrite(buf, 1, 512, fout);
		tot ++;
	}

	if(nr_block > 12) {
		tot ++;
		if(nr_block > 12 + 128) {
			tot ++;
			tot += min(128, (nr_block - 12 - 128 - 1) / 128 + 1);
		}
	}
	if(tot > 1024 * 1024 * size / 512) {
		printf("File size is larger than disk size!!!\n");
		assert(0);
	}
}
int main(int argc, char ** args) {
	int i;
	memset(buf, 0, sizeof buf);
	size = atoi(args[1]);
	fout = fopen("obj/disk", "wb");
	for(i = 0; i < 1024 * 1024 * size / 512; ++ i) fwrite(buf, 1, 512, fout);
	for(i = 2; i < argc; ++ i) {
		create_file(args[i]);
	}

	for(i = 0; i < tot; ++ i) {
		map[i / 8] |= 1 << (i & 0x7);
	}
	fseek(fout, 0, SEEK_SET);
	fwrite(map, 1, 512, fout);
	fwrite(&d[0], 1, 512, fout);
	return 0;
}
