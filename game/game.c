int printk(const char *fmt, ...);
void testprintk();
int game_init(){
	printk("@_@\n");
	testprintk();
	while(1);
	return 0;
}
