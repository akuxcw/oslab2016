#include <inc/x86.h>
#include <inc/elf.h>
#include <inc/stdio.h>
#define SECTSIZE 512

void readseg(unsigned char *, int, int);

void init_i8259();
void init_timer();
void init_serial();
void init_idt();

void init_palette();

int kern_main() {
	init_timer();
	init_i8259();
	init_serial();
	init_idt();
	init_palette();

	struct Elf *elf;
	struct Proghdr *ph, *eph;
	unsigned char* pa, *i;

	elf = (struct Elf*)(0x19200);

	readseg((unsigned char*)elf, 4096, 0);

	printk("%x\n", *(int*)elf);

	ph = (struct Proghdr*)((uint8_t *)elf + elf->e_phoff);
	eph = ph + elf->e_phnum;
	for(; ph < eph; ph ++) {
		pa = (unsigned char*)ph->p_pa; 
		readseg(pa, ph->p_filesz, ph->p_offset); 
		for (i = pa + ph->p_filesz; i < pa + ph->p_memsz; *i ++ = 0);
	}

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
