#include <stdio.h>
#include <time.h>
void game();

int game_main(){
//	asm volatile("sti");
//	time();
	printf("@_@ Game Start!\n");
	game();
	while(1);
	return 0;
}
