#include <stdio.h>
#include <time.h>
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
	while(1) {
		printf("ping\n");
		sleep(100);
	}
	return 0;
}
