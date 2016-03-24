#include <stdio.h>
#include <time.h>
void game();

void init_kbd();

int game_main(){
//	asm volatile("sti");
	time();
	printf("@_@ Game Start!\n");
	init_kbd();
	game();
	while(1);
	return 0;
}
