int printk(const char *fmt, ...);
void testprintk();
void init_vga();
void init_i8259();
void init_seriel();
int syscall(int id, ...);

static void add_irq_handle(int irq, void *handler) {
	syscall(0, irq, handler);
}

volatile int tick = 0;

void timer_event(void) {
	tick ++;
}
void kbd_event(void);


int game_init(){
	printk("@_@\n");
	init_i8259();
	add_irq_handle(0, timer_event);
	add_irq_handle(0, kbd_event);

	init_vga();
	while(1);
	return 0;
}
