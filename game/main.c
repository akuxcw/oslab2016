#include <inc/stdio.h>

void testprintk();
void game();

int game_main(){
	printf("@_@ Game Start!\n");
	asm volatile("sti");
	game();
	while(1);
	return 0;
}
