#include <common.h>
#include <inc/x86.h>
#include <inc/elf.h>
#include <inc/mmu.h>
#include "inc/irq.h"
#include <inc/memory.h>
#include "inc/disk.h"
#include <inc/fs.h>

#define OFFSET_IN_DISK 1000*1024

uint32_t mm_malloc(uint32_t, uint32_t, PCB*);
void set_segment(SegDesc *ptr, uint32_t pl, uint32_t type, uint32_t base, uint32_t limit);

void set_user_page();
void set_kern_page();
void set_kern_segment();
void readprog(uint32_t, uint32_t, uint32_t, PCB*, unsigned char *, uint32_t);
void init_i8259();
void init_timer();
void init_serial();
void init_idt();
void init_palette();
void init_memory();
void init_sem();
void load();

uint8_t buf[512];

void testfs() {
	ide_read(OFFSET_IN_DISK/SECTSIZE, buf, 1);
	int i;
	printk("testfs\n");
	for(i = 0; i < 10; ++ i)
		printk("%d\n", buf[i]);
}

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
	init_sem();				//init semaphore
	
	testfs();
	while(1);	
	load();					//load program
	sti();
	while(1);
	return 0;
}

void load() {
	PCB *current = new_process();
	set_user_page(current);
	TrapFrame *tf = (TrapFrame *)current->kstack;
	current->tf = tf;
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
//	lcr3(va2pa(current->pdir));

}

