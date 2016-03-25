#include <common.h>
#include <inc/x86.h>
#include <inc/elf.h>
#include <inc/mmu.h>
#include "inc/irq.h"
#include "inc/process.h"
#include "inc/disk.h"

#define OFFSET_IN_DISK 0x19000


void set_tss_esp0(int);
void set_segment(SegDesc *ptr, uint32_t pl, uint32_t type, uint32_t base, uint32_t limit);

void init_i8259();
void init_timer();
void init_serial();
void init_idt();
void init_segment();
void init_palette();
void init_memory();
void load();

int kern_main() {
	init_segment();			//set segment register for kernel
	init_timer();			//init time interrupt
	init_i8259();			//init interrupt
	init_serial();			//init serial
	init_idt();				//set idt
	init_palette();			//set palette address
	init_process();			//init PCB pointer
	init_memory();			//init memory organize
	load();					//load program
	outw(0x8A00, 0x8A00);
	outw(0x8A00, 0x8E00);
	while(1);
	return 0;
}

void load() {
	PCB *current = new_process();
	
	struct Elf *elf;
	struct Proghdr *ph, *eph;
	unsigned char* pa, *i;

	elf = (struct Elf*)(0x19000);

	readseg((unsigned char*)elf, 4096, OFFSET_IN_DISK);

	ph = (struct Proghdr*)((uint8_t *)elf + elf->e_phoff);
	eph = ph + elf->e_phnum;
	uint32_t cnt = 0;
	for(; ph < eph; ph ++) {
		cnt ++;
		pa = (unsigned char*)ph->p_pa + cnt * SEG_OFFSET; 
		readseg(pa, ph->p_filesz, OFFSET_IN_DISK + ph->p_offset); 
		for (i = pa + ph->p_filesz; i < pa + ph->p_memsz; *i ++ = 0);
	}
	

	uint32_t eflags = read_eflags();

	TrapFrame *tf = &current->tf;
	set_tss_esp0((int)current->kstack + KSTACK_SIZE);
	tf->gs = tf->fs = tf->es = tf->ds = SELECTOR_USER(SEG_USER_DATA);
	tf->eax = 0; tf->ebx = 1; tf->ecx = 2; tf->edx = 3;
	
	tf->eflags = eflags | FL_IF;
	tf->eip = elf->e_entry;
	tf->cs = SELECTOR_USER(SEG_USER_CODE);
	tf->ss = SELECTOR_USER(SEG_USER_DATA);
	tf->esp = 0x2000000;
	asm volatile("movl %0, %%esp" : :"a"((int)tf));
	asm volatile("popa");
	asm volatile("addl %0, %%esp" : :"a"(8));
	asm volatile("movl %0, %%ds\n\t"
				 "movl %0, %%es\n\t"
				 "movl %0, %%fs\n\t"
				 "movl %0, %%gs\n\t" 
				 : 
				 : "a"(SELECTOR_USER(SEG_USER_DATA)));
	asm volatile("iret");


}

