#include <stdio.h>
#include <fs.h>

char buf[1024];
char a[] = "Hello FS.";
int game_main() {
	printf("Test file system.\n");
	int fin = fopen("a/1.txt", READ);
	int fout = fopen("1.txt", WRITE);
	fseek(fout, 10, SEEK_SET);
	fwrite(fout, a, 9);
	fread(fin, buf, 100);
	printf("%s", buf);
	while(1);
	return 0;
}
