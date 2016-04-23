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
	fork();
	fork();
	printf("*\n");
		
/*
	int i;
	for(i = 0; i < 2; ++ i) {
		int x = fork();
		printf("%d\n", x);
	}
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
	*/
	return 0;
}
#endif
