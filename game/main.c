#include <stdio.h>
#include <time.h>
#include <process.h>
void game();

//#define TEST_FORK

#ifndef TEST_FORK
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
#else
int game_main() {
	int pid = fork();
	int x = 0;
	while(1) {
		x ++;
		if(pid == 0) {
			printf("ping %x %x\n", pid, x);
			sleep(100);
		} else {
			printf("pang %x %x\n", pid, x);
			sleep(200);
		}
	}
	return 0;
}
#endif
