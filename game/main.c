#include <stdio.h>
#include <time.h>
void game();

int game_main(){
//	asm volatile("sti");
	int addr = 0x16040000;
	*(int *)addr = 1;
	printf("****%d\n", *(int *)addr);
	time();
	printf("@_@ Game Start!\n");
	game();
	while(1);
	return 0;
}
