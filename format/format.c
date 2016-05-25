#include "fs.h"
#include <stdio.h>
#include <string.h>

dir root;
char map[512];
int index0[MAXFILESIZE / SECTSIZE];
int index1[MAXFILESIZE / SECTSIZE];

int min(int a, int b) {
	return a < b ? a : b;
}

int main(int argc, char ** args) {
	FILE *fout;
	int i, tot = 2, size = atoi(args[1]);
	char buf[512];
	memset(buf, 0, sizeof buf);
	fout = fopen("obj/disk", "wb");
	for(i = 0; i < 1024 * 1024 * size / 512; ++ i) fwrite(buf, 1, 512, fout);
	for(i = 2; i < argc; ++ i) {
		int j, nr_block;
		FILE *fin;
		memset(index0, 0, sizeof index0);
		memset(index1, 0, sizeof index1);

		fin = fopen(args[i], "rb");
		fseek(fin, 0, SEEK_END);
		int len = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		
		if(len > MAXFILESIZE) {
			printf("File %s is too large.", args[i]);
			return 0;
		}
		nr_block = (len - 1) / SECTSIZE + 1;
//		printf("%d\n", nr_block);
		for(j = 0; j < nr_block; ++ j) {
			index0[j] = tot + j + 1;
		}

		strcpy(root.entry[i-2].filename, &args[i][5]);
		root.entry[i-2].filesz = len;
		root.entry[i-2].inode_off = tot;

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
	}

	for(i = 0; i < tot; ++ i) {
		map[i / 8] |= 1 << (i & 0x7);
	}
	fseek(fout, 0, SEEK_SET);
	fwrite(map, 1, 512, fout);
	fwrite(&root, 1, 512, fout);
	return 0;
}
