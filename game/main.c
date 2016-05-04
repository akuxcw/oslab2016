#include <stdio.h>
#include <time.h>
#include <process.h>

#define TEST_FORK

#ifndef TEST_FORK

void game();

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

	printf("@_@ Game Start!\n");
/*
	int x1 = fork();
	int x2 = fork();
	if(x1 == 0) exit(0);
	fork();
	if(x2 == 0) exit(0);
	printf("Done!\n");
	while(1);
*/

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

	int i;
	for(i = 0; i < 2; ++ i) {
		fork();
		printf("x");
	}
	while(1);
	return 0;

}
#endif
