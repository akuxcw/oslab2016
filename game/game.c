int printk(const char *fmt, ...);
void testprintk();
void init_vga();
void init_i8259();
void init_timer();
void init_serial();

/*
int syscall(int id, ...);
static void add_irq_handle(int irq, void *handler) {
	syscall(0, irq, handler);
}

void timer_event(void); 
void kbd_event(void);
*/
int game_init(){
	init_timer();
	init_i8259();
	init_serial();
//	add_irq_handle(0, timer_event);
//	add_irq_handle(0, kbd_event);

	printk("@_@\n");
	init_vga();
	while(1);
	return 0;
}
