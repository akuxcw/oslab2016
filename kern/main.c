
void init_i8259();
void init_timer();
void init_serial();
void init_idt();

void init_palette();
//void game_main();

int kern_main() {
	init_timer();
	init_i8259();
	init_serial();
	init_idt();
	init_palette();
//	game_main();
	return 0;
}
