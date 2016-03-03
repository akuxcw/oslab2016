int printk(const char *fmt, ...);
void testprintk();
void init_vga();
void init_i8259();
void init_timer();
void init_serial();
void init_idt();

void set_timer_intr_handler( void (*ptr)(void));
void set_kbd_intr_handler( void (*ptr)(int));

void timer_event(void); 
void kbd_event(int);

int game_init(){
	init_timer();
	init_i8259();
	init_serial();
	init_idt();
	set_timer_intr_handler(timer_event);
	set_kbd_intr_handler(kbd_event);

	printk("@_@\n");
	asm volatile("sti");
	init_vga();
	while(1);
	return 0;
}
