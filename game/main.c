#include <stdio.h>
#include <time.h>
#include <process.h>
void game();
/*
int game_main() {
//	asm volatile("sti");
//	int addr = 0x4000;
//	*(int *)addr = 1;
//	printf("****%d\n", *(int *)addr);

	sleep(100);
	time();
	printf("@_@ Game Start!\n");
	game();
	while(1);
	return 0;
}
*/
int game_main() {
	int pid = fork();
	while(1) {
		if(pid == 0) {
			printf("ping\n");
			sleep(100);
		} else {
			printf("pang\n");
			sleep(200);
		}
	}
	return 0;
}
