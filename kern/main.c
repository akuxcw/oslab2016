
void init_palette();
void game_main();

int kern_main() {
	init_palette();
	game_main();
	return 0;
}
