#include "../irq/irq.h"
#include <inc/syscall.h>
#include <inc/string.h>
#include <inc/stdio.h>
#include <inc/mmu.h>
enum {KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS};
/*
void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);
void serial_printc(char);

int fs_open(const char *pathname, int flags);
int fs_read(int fd, void *buf, int len);
int fs_write(int fd, void *buf, int len);
int fs_lseek(int fd, int offset, int whence);
int fs_close(int fd);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}
*/

void serial_putc(char);

static void sys_write(TrapFrame *tf) {
	int i;
	for(i = 0; i < tf->edx; ++ i)
		serial_putc(*(char *)(tf->ecx + SEG_OFFSET + i));
	tf->eax = tf->edx;
}
extern char * color_buffer;
static void sys_palette(TrapFrame *tf) {
	memcpy(color_buffer, (void *)(tf->ebx + SEG_OFFSET), 800*600*3);
}

int query_key(int);

static void sys_kbd(TrapFrame *tf) {
	int i;
	int *kbd = (int *)(char *)(tf->ebx + SEG_OFFSET);
	for(i = 0; i < tf->ecx; ++ i) {
//		printk("%d %d\n", kbd[i], query_key(i));
		if(kbd[i] == KEY_STATE_EMPTY && query_key(i) == KEY_STATE_PRESS) kbd[i] = KEY_STATE_PRESS; else
		if(kbd[i] == KEY_STATE_WAIT_RELEASE && query_key(i) == KEY_STATE_RELEASE) kbd[i] = KEY_STATE_RELEASE;
	}
}

int Get_time();
void Delay(int);

void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
/*		case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;

		case SYS_brk: panic("@@@"); sys_brk(tf); break;
*/
		case SYS_write: sys_write(tf); break;
		case SYS_palette: sys_palette(tf); break;
		case SYS_kbd: sys_kbd(tf); break;
		case SYS_time: tf->eax = Get_time(); break;
		case SYS_sleep: Delay(tf->ebx); break;
/*
		case SYS_open : 
			tf->eax = fs_open((char *)tf->ebx, tf->ecx); break;
		case SYS_read : 
//			Log("%x %x %x %x %x\n", tf->ebx, tf->ecx, tf->edx, tf->edi, tf->esi);
			tf->eax = fs_read(tf->ebx, (void *)tf->ecx, tf->edx); break;
		case SYS_lseek : tf->eax = fs_lseek(tf->ebx, tf->ecx, tf->edx); break;
		case SYS_close : tf->eax = fs_close(tf->ebx); break;
*/

		default: printk("Unhandled system call: id = %d", tf->eax);
	}
	asm volatile("movl %0, %%es" : : "a"(SELECTOR_USER(SEG_USER_DATA)));
	asm volatile("movl %0, %%ds" : : "a"(SELECTOR_USER(SEG_USER_DATA)));
	asm volatile("movl %0, %%fs" : : "a"(SELECTOR_USER(SEG_USER_DATA)));
	asm volatile("movl %0, %%gs" : : "a"(SELECTOR_USER(SEG_USER_DATA)));

}

