#include <common.h>
#include <inc/x86.h>
#include <inc/elf.h>
#include <inc/mmu.h>
#include "inc/irq.h"
#include "inc/process.h"
#include "inc/disk.h"
//#include <inc/memory.h>

#define OFFSET_IN_DISK 1000*1024
#define USER_STACK_TOP 0x8000000
#define USER_STACK_SIZE 0x400000

uint32_t mm_malloc(uint32_t, uint32_t, PCB*);
void set_tss_esp0(int);
void set_segment(SegDesc *ptr, uint32_t pl, uint32_t type, uint32_t base, uint32_t limit);

void set_user_page();
void set_kern_page();
void set_kern_segment();
uint32_t seg_alloc(uint32_t, PCB*);
uint32_t page_alloc(uint32_t, uint32_t, PCB*);
void readprog(uint32_t, uint32_t, uint32_t, PCB*, unsigned char *, uint32_t);
void init_i8259();
void init_timer();
void init_serial();
void init_idt();
void init_palette();
void init_memory();
void load();

int kern_init() {
	init_palette();			//set palette address
	set_kern_page();
	set_kern_segment();			//set segment register for kernel
	init_timer();			//init time interrupt
	init_i8259();			//init interrupt
	init_serial();			//init serial
	init_idt();				//set idt
	init_process();			//init PCB pointer
	init_memory();			//init memory organize
	
/*	int *addr = (int *)0xf0000000;
	*addr = 1;
	printk("$$$%d\n", *addr);
*/	load();					//load program
	outw(0x8A00, 0x8A00);
	outw(0x8A00, 0x8E00);
	while(1);
	return 0;
}

void load() {
	PCB *current = new_process();
	set_user_page(current);
	struct Elf *elf;
	struct Proghdr *ph, *eph;
	unsigned char* pa;
#ifndef USE_PAGE
	uint32_t vaddr;
#endif
	elf = (struct Elf*)(0x0019000);

	readseg((unsigned char*)elf, 4096, OFFSET_IN_DISK);
	printk("Welcome to my OS!\n");

	ph = (struct Proghdr*)((uint8_t *)elf + elf->e_phoff);
	eph = ph + elf->e_phnum;
	for(; ph < eph; ph ++) {
		if(ph->p_type != ELF_PROG_LOAD) continue;
//		pa = (unsigned char *)mm_malloc(ph->p_va, ph->p_memsz/* + 0xb00000*/, current);
//		printk("**********************\n");

//		printk("%x\n", pa);

		pa = (unsigned char *)seg_alloc(ph->p_va, current);
#ifdef USE_PAGE
		pa = (unsigned char *)page_alloc(ph->p_va, ph->p_memsz, current);
#else
		vaddr = ph->p_va;
#endif
//		printk("%x %x\n", OFFSET_IN_DISK + ph->p_offset, pa);
		readprog(ph->p_va, ph->p_filesz, ph->p_memsz, current, pa, OFFSET_IN_DISK + ph->p_offset);

//		readseg(pa, ph->p_filesz, OFFSET_IN_DISK + ph->p_offset); 
//		unsigned char *i;
	//	for (i = pa + ph->p_filesz; i < pa + ph->p_memsz; *i ++ = 0);
	}
	
	printk("Ready to game!\n");

	uint32_t eflags = read_eflags();

	TrapFrame *tf = &current->tf;
	set_tss_esp0((int)current->kstack + KSTACK_SIZE);
	tf->eax = 0; tf->ebx = 1; tf->ecx = 2; tf->edx = 3;
	
	tf->eflags = eflags | FL_IF;
	tf->eip = elf->e_entry;
#ifdef USE_PAGE
	tf->esp = USER_STACK_TOP;
//	mm_malloc(USER_STACK_TOP - USER_STACK_SIZE, USER_STACK_SIZE, current);
	page_alloc(USER_STACK_TOP - USER_STACK_SIZE, USER_STACK_SIZE, current);
//	printk("!!!!!\n");
#else
	tf->esp = 0x2000000 - (int)pa + vaddr;
#endif

//	int j;
//	for(j = 0; j < 1024; ++ j) printk("%x\n", current->pdir[j]);
//	for(j = 0; j < 0x3000000/PGSIZE; ++ j) printk("%x %x\n", (int)&current->ptable[j], current->ptable[j]);
	ready(current);
	exec();
	lcr3(va2pa(current->pdir));
	asm volatile("movl %0, %%esp" : :"a"((int)tf));
	asm volatile("popa");
	asm volatile("addl %0, %%esp" : :"a"(8));

	asm volatile("mov 24(%esp), %eax\n\t"
				 "movl %eax, %ds\n\t"
				 "movl %eax, %es\n\t"
				 "movl %eax, %fs\n\t"
				 "movl %eax, %gs\n\t");
	//while(1);
	asm volatile("iret");
}

