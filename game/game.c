int printk(const char *fmt, ...);
void testprintk();
void init_vga();

int game_init(){
	printk("@_@\n");
//	testprintk();
	init_vga();
	while(1);
	return 0;
}
