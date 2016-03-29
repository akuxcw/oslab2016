#include <common.h>
#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/memory.h>
#include <inc/string.h>
#include <inc/x86.h>

__attribute__((__aligned__(PGSIZE)))
pde_t kpdir[NPDENTRIES];			// kernel page directory

__attribute__((__aligned__(PGSIZE)))
pte_t kptable[(2 * MAX_MEM + 0x1000000) / PGSIZE];		// kernel page tables

PgMan page[MAX_MEM / PGSIZE];

ListHead free_pg;
ListHead used_pg;

void set_kern_page() {
/*	asm volatile("movl	%cr0, %eax\n\t"
				 "andl	$0x7fffffff, %eax\n\t"
				 "movl	%eax, %cr0");
*/
	uint32_t pdir_idx;

	pde_t * pdir = kpdir;// - KERNBASE;
	pte_t * ptable = kptable;// - KERNBASE;

	memset(pdir, 0, NPDENTRIES * sizeof(pte_t));

	for (pdir_idx = 0; pdir_idx < MAX_MEM / PTSIZE; pdir_idx ++) {
		pdir[pdir_idx] = (pde_t)ptable | 0x7;
		pdir[pdir_idx + KERNBASE / PTSIZE] = (pde_t)ptable | 0x7;
//		printk("%x\n", pdir[pdir_idx]);
		ptable += 1024;//NPDENTRIES;
	}
	for(pdir_idx = 0xfc000000 / PTSIZE; pdir_idx < 0xfd000000 / PTSIZE; pdir_idx ++) {
		pdir[pdir_idx] = (pde_t)ptable | 0x7;
//		printk("%x\n", pdir[pdir_idx]);
		ptable += 1024;
	}
	printk("%d\n", pdir_idx);
//	printk("**************\n");
/*	asm volatile ("std;\
	 1: stosl;\
		subl %0, %%eax;\
		jge 1b" : : 
		"i"(PGSIZE), "a"((MAX_MEM - PGSIZE) | 0x7), "D"(ptable - 1));
*/
	int32_t pframe_addr;
	ptable --;
	
		// fill PTEs reversely
	for (pframe_addr = 0xfd000000 - PGSIZE; pframe_addr >= 0xfc000000; pframe_addr -= PGSIZE) {
		*ptable = (pte_t)pframe_addr | 0x7;
//		printk("%x %x\n", (int)ptable, *ptable);
		ptable --;
	}
	for (pframe_addr = MAX_MEM - PGSIZE; pframe_addr >= 0; pframe_addr -= PGSIZE) {
		*ptable = (pte_t)pframe_addr | 0x7;
//		printk("%x %x\n", (int)ptable, *ptable);
		ptable --;
	}
//	printk("***$$$$$$$$$***********\n");
	lcr3((uint32_t)pdir/* - 0xf0000000*/);
	asm volatile("movl	%cr0, %eax\n\t"
				 "orl	$0x80010001, %eax\n\t"
				 "movl	%eax, %cr0\n\t");

//	while(1);
}


void init_page() {
	int i, tot = 0;
	list_init(&free_pg);
	list_init(&used_pg);
	for(i = 0x400000; i < MAX_MEM; i += PGSIZE) {
		page[tot].addr = i;
		list_add_before(&free_pg, &page[tot].list);
	}
}
