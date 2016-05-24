#include <stdio.h>
#include <fs.h>

char buf[1024];

int game_main() {
	printf("Test file system.\n");
	int fd = fopen("1.txt", READ);
	fread(fd, buf, 100);
	printf("%s", buf);
	while(1);
	return 0;
}
