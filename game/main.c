#include <inc/stdio.h>

void testprintk();
void init_i8259();
void init_timer();
void init_serial();
void init_idt();

void set_timer_intr_handler( void (*ptr)(void));

void timer_event(void); 

void game();

int game_main(){
	init_timer();
	init_i8259();
	init_serial();
	init_idt();
	set_timer_intr_handler(timer_event);

	printf("@_@ Game Start!\n");
	asm volatile("sti");
	game();
	while(1);
	return 0;
}
