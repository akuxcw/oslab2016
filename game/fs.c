#include <stdio.h>
#include <fs.h>
#include <string.h>

char buf[1024];
char a[] = "Hello FS.";
int game_main() {
	printf("Test file system.\n");
	int fin = fopen("a/1.txt", READ);
	int fout = fopen("a/1.txt", WRITE);
	fseek(fout, 10, SEEK_SET);
	fwrite(fout, a, 9);
	fread(fin, buf, 100);
	printf("%s", buf);
	while(1);
/*	int fin = fopen("1.txt", READ);
	int fout = fopen("2.txt", WRITE);
	fread(fin, buf, 600);
	printf("%s\n", buf);
	fwrite(fout, buf, 600);
	int fin2 = fopen("2.txt", READ);
	memset(buf, '\0', sizeof buf);
	fread(fin2, buf, 600);
	printf("%s\n", buf);
	while(1);
*/	return 0;
}
