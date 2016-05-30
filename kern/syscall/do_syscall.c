#include "inc/irq.h"
#include <inc/syscall.h>
#include <inc/assert.h>
#include <inc/string.h>
#include <inc/stdio.h>
#include <inc/mmu.h>
#include <inc/memory.h>
#include <inc/x86.h>
#include <inc/sem.h>

enum {KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS};
/*
void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);
void serial_printc(char);
*/
int fopen(char *name, int flags);
int fread(int fd, void *buf, int len);
int fwrite(int fd, void *buf, int len);
int fseek(int fd, int offset, int whence);
int fclose(int fd);
/*
static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}
*/

uint32_t Get_seg_off();
void serial_putc(char);

static void sys_write(TrapFrame *tf) {
	int i;
	for(i = 0; i < tf->edx; ++ i)
		serial_putc(*(char *)(tf->ecx + Get_seg_off() + i));
	tf->eax = tf->edx;
}
extern char * color_buffer;
static void sys_palette(TrapFrame *tf) {
	memcpy(color_buffer, (void *)(tf->ebx + Get_seg_off()), 800*600*3);
}

int query_key(int);

static void sys_kbd(TrapFrame *tf) {
	int i;
	int *kbd = (int *)(tf->ebx + Get_seg_off());
	for(i = 0; i < tf->ecx; ++ i) {
//		printk("%d %d\n", kbd[i], query_key(i));
		if(kbd[i] == KEY_STATE_EMPTY && query_key(i) == KEY_STATE_PRESS) kbd[i] = KEY_STATE_PRESS; else
		if(kbd[i] == KEY_STATE_WAIT_RELEASE && query_key(i) == KEY_STATE_RELEASE) kbd[i] = KEY_STATE_RELEASE;
	}
}

int Get_time();
extern void exit(int);
void thread_create(int *, int, int);
//void thread_join(int, int);

void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		case SYS_write: sys_write(tf); break;
		case SYS_palette: sys_palette(tf); break;
		case SYS_kbd: sys_kbd(tf); break;
		case SYS_time: tf->eax = Get_time(); break;
		case SYS_sleep: /*cli();*/ sleep(current, tf->ebx); /*sti();*/ break;
		
		case SYS_fork: /*cli();*/ tf->eax = fork(); /*sti();*/break;
		case SYS_exit: exit(tf->ebx); break;

		case SYS_thread_create: thread_create((int *)tf->ebx, tf->ecx, tf->edx); break;
//		case SYS_thread_join: thread_join(tf->ebx, tf->ecx); break;

		case SYS_sem_open: tf->eax = sem_open((char *)tf->ebx, tf->ecx, (bool)tf->edx); break;
		case SYS_sem_close: sem_close(tf->ebx); break;
		case SYS_sem_wait: sem_wait(tf->ebx); break;
		case SYS_sem_post: sem_post(tf->ebx); break;

		case SYS_fopen : 
			tf->eax = fopen((char *)tf->ebx, tf->ecx); break;
		case SYS_fread : 
			tf->eax = fread(tf->ebx, (void *)tf->ecx, tf->edx); 
			printk("%s\n", (char *)tf->ecx);break;
		case SYS_fwrite : 
			tf->eax = fwrite(tf->ebx, (void *)tf->ecx, tf->edx); break;
		case SYS_fseek : tf->eax = fseek(tf->ebx, tf->ecx, tf->edx); break;
		case SYS_fclose : tf->eax = fclose(tf->ebx); break;


		default: panic("Unhandled system call: id = %d\n", tf->eax);
	}
}

