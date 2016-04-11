#include <inc/stdio.h>
#include <inc/assert.h>
#include <inc/x86.h>
#include <inc/mmu.h>
#include <inc/process.h>
#include "inc/irq.h"

uint32_t Get_gdt_off(uint32_t);
uint32_t Get_cr3();
void do_timer(void);
void do_kbd(int);

void do_syscall(TrapFrame *);

int offset;

void
irq_handle(TrapFrame *tf) {
	//cli();
	int seg_tmp;
	asm volatile("movl %%es, %0" : "=a"(seg_tmp) :);
	asm volatile("movl %0, %%es\n\t"
				 "movl %0, %%ds\n\t"
				 "movl %0, %%fs\n\t"
				 "movl %0, %%gs\n\t" 
				 : 
				 : "a"(SELECTOR_KERNEL(SEG_KERNEL_DATA)));
	offset = Get_gdt_off(seg_tmp >> 3);
	bool from_user = false;
	if(seg_tmp & 0x3) from_user = true;
	uint32_t code, val;
	if(tf->irq < 1000) {
		if(tf->irq == -1) {
			panic("Unhandled exception!\n");
		} 
		else if(tf->irq == 0x80) {
			do_syscall(tf);
		}
		else {
			panic("Unexpected exception #%d %d!\n", tf->irq, tf->error_code);
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
			default : panic("Error in irq_handle.c : %d\n", tf->irq);
	}
	if(from_user) exec();
	asm volatile("movl %0, %%es\n\t"
				 "movl %0, %%ds\n\t"
				 "movl %0, %%fs\n\t"
				 "movl %0, %%gs" : : "a"(seg_tmp));
	//sti();
}

uint32_t Get_seg_off() {
	return offset;
}
