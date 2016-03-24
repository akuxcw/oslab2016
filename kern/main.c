#include <inc/x86.h>
#include <inc/elf.h>
#include <inc/stdio.h>
#include <inc/mmu.h>
#include "irq/irq.h"

#define SECTSIZE 512
#define OFFSET_IN_DISK 0x19000

void readseg(unsigned char *, int, int);

void init_i8259();
void init_timer();
void init_serial();
void init_idt();
void init_segment();
void init_palette();

int kern_main() {
	init_segment();
	init_timer();
	init_i8259();
	init_serial();
	init_idt();
	init_palette();
	
//	loader
	struct Elf *elf;
	struct Proghdr *ph, *eph;
	unsigned char* pa, *i;

	elf = (struct Elf*)(0x19000);

	readseg((unsigned char*)elf, 4096, OFFSET_IN_DISK);


	ph = (struct Proghdr*)((uint8_t *)elf + elf->e_phoff);
	eph = ph + elf->e_phnum;
	for(; ph < eph; ph ++) {
		pa = (unsigned char*)ph->p_pa + SEG_OFFSET; 
		readseg(pa, ph->p_filesz, OFFSET_IN_DISK + ph->p_offset); 
		for (i = pa + ph->p_filesz; i < pa + ph->p_memsz; *i ++ = 0);
	}

	printk("Ready to game!\n");

	uint32_t eflags = read_eflags();

	TrapFrame_iret *tf = (TrapFrame_iret *)0x10000;
	tf->gs = tf->fs = tf->es = tf->ds = SELECTOR_USER(SEG_USER_DATA);
	tf->eax = 0; tf->ebx = 1; tf->ecx = 2; tf->edx = 3;
	
	tf->eflags = eflags | (1 << 9);
	tf->eip = elf->e_entry + SEG_OFFSET;
	tf->cs = (SEG_USER_CODE << 3) | 0x3;//SELECTOR_USER(SEG_USER_CODE);
	tf->ss = (SEG_USER_CODE << 3) | 0x3;//SELECTOR_USER(SEG_USER_DATA);
	tf->esp = 0x8000000;
	asm volatile("movl %0, %%esp" : :"a"((int)tf));
	asm volatile("popa");
	asm volatile("addl %0, %%esp" : :"a"(24));
//	asm volatile("movl %0, %%ds" : :"a"(SELECTOR_USER(SEG_USER_DATA)));
//	asm volatile("movl %0, %%es" : :"a"(SELECTOR_USER(SEG_USER_DATA)));
	asm volatile("iret");
		((void(*)(void))elf->e_entry)();

	outw(0x8A00, 0x8A00);
	outw(0x8A00, 0x8E00);
	while(1);
	return 0;
}

void waitdisk(void) {
	while((inb(0x1F7) & 0xC0) != 0x40); 
}

void readsect(void *dst, int offset) {
	waitdisk();
	outb(0x1F2, 1);
	outb(0x1F3, offset);
	outb(0x1F4, offset >> 8);
	outb(0x1F5, offset >> 16);
	outb(0x1F6, (offset >> 24) | 0xE0);
	outb(0x1F7, 0x20);

	waitdisk();
	insl(0x1F0, dst, SECTSIZE/4);
}

void readseg(unsigned char *pa, int count, int offset) {
	unsigned char *epa;
	epa = pa + count;
	pa -= offset % SECTSIZE;
	offset = (offset / SECTSIZE) + 1;
	for(; pa < epa; pa += SECTSIZE, offset ++)
		readsect(pa, offset);
}
