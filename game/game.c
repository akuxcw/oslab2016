int printk(const char *fmt, ...);
void testprintk();
void init_vga();

int game_init(){
	printk("@_@\n");
//	testprintk();
	printk("%d\n", (int *)0xa0000);
	init_vga();
	while(1);
	return 0;
}
