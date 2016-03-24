#include <inc/stdio.h>
#include <inc/assert.h>
#include <inc/x86.h>
#include <inc/mmu.h>
#include "irq.h"

void do_timer(void);
void do_kbd(int);

void do_syscall(TrapFrame *);

void
irq_handle(TrapFrame *tf) {
	asm volatile("movl %0, %%es" : : "a"(SELECTOR_KERNEL(SEG_KERNEL_DATA)));
	asm volatile("movl %0, %%ds" : : "a"(SELECTOR_KERNEL(SEG_KERNEL_DATA)));
	asm volatile("movl %0, %%fs" : : "a"(SELECTOR_KERNEL(SEG_KERNEL_DATA)));
	asm volatile("movl %0, %%gs" : : "a"(SELECTOR_KERNEL(SEG_KERNEL_DATA)));
	uint32_t code, val;
	if(tf->irq < 1000) {
		if(tf->irq == -1) {
			printk("%s, %d: Unhandled exception!\n", __FUNCTION__, __LINE__);
		} 
		else if(tf->irq == 0x80) {
			do_syscall(tf);
		}
		else {
			printk("%s, %d: Unexpected exception #%d %d!\n", __FUNCTION__, __LINE__, tf->irq, tf->error_code);
		}
	}
	else 
		switch (tf->irq) {
			case 1000:
				do_timer();
				break;
			case 1001: 
				code = inb(0x60);
				val = inb(0x61);
				outb(0x61, val | 0x80);
				outb(0x61, val);
				//printk("%s, %d: key code = %x\n", __FUNCTION__, __LINE__, code);
				do_kbd(code);
			case 1014 :
				break;
			default : printk("Error in irq_handle.c : %d\n", tf->irq);
	}
	asm volatile("movl %0, %%es" : : "a"(SELECTOR_USER(SEG_USER_DATA)));
	asm volatile("movl %0, %%ds" : : "a"(SELECTOR_USER(SEG_USER_DATA)));
	asm volatile("movl %0, %%fs" : : "a"(SELECTOR_USER(SEG_USER_DATA)));
	asm volatile("movl %0, %%gs" : : "a"(SELECTOR_USER(SEG_USER_DATA)));

}

