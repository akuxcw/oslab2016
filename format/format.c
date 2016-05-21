#include "fs.h"
#include <stdio.h>
#include <string.h>

dir root;
char map[512];

int main(int argc, char ** args) {
	FILE *fout;
	int i, tot = 2;
	char buf[512];
//	printf("%d\n", sizeof map);
	fout = fopen("disk", "wb");
//	buf[0] = 'a';
//	buf[1] = 'b';
//	fseek(fout, 10, SEEK_SET);
//	fwrite(buf, 1, 2, fout);
//	return 0;

	for(i = 1; i < argc; ++ i) {
		FILE *fin;
		fin = fopen(args[i], "rb");
		fseek(fin, 0, SEEK_END);
		int len = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		strcpy(root.entry[i].filename, args[i]);
		root.entry[i].filesz = len;
		root.entry[i].inode_off = tot;
		inode tmp;
		memset(&tmp, 0, sizeof tmp);
		int j;
		for(j = 0; j < (len - 1) / 512 + 1; j ++) {
			tmp.index[j] = tot + j + 1;
		}
		fseek(fout, tot * 512, SEEK_SET);
		tot ++;
		fwrite(&tmp, 1, 512, fout);
		for(j = 0; j < len; j += 512) {
			fread(buf, 1, 512, fin);
			fwrite(buf, 1, 512, fout);
			tot ++;
		}
	}
	for(i = 0; i < tot; ++ i) map[i] = 1;
	fseek(fout, 0, SEEK_SET);
	fwrite(map, 1, 512, fout);
	fwrite(&root, 1, 512, fout);
	return 0;
}
