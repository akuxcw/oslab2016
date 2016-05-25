#include <stdio.h>
#include <fs.h>

char buf[1024];
char a[] = "Hello FS.";
int game_main() {
	printf("Test file system.\n");
	int fin = fopen("a/a1", READ);
	int fout = fopen("a/a1", WRITE);
	fseek(fout, 10, SEEK_SET);
	fwrite(fout, a, 9);
	fread(fin, buf, 100);
	printf("%s", buf);
	while(1);
	return 0;
}
