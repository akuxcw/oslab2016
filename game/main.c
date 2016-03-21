#include <inc/stdio.h>

void game();

int game_main(){
	asm volatile("sti");
	printf("@_@ Game Start!\n");
	game();
	while(1);
	return 0;
}
