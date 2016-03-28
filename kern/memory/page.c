#include <common.h>
#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/memory.h>
#include <inc/string.h>
#include <inc/x86.h>

__attribute__((__aligned__(PGSIZE)))
pde_t kpdir[NPDENTRIES];			// kernel page directory

__attribute__((__aligned__(PGSIZE)))
pte_t kptable[MAX_MEM / PGSIZE];		// kernel page tables

PgMan page[MAX_MEM / PGSIZE];

ListHead free_pg;
ListHead used_pg;

void set_kern_page() {
	uint32_t pdir_idx;

	pde_t * pdir = kpdir - KERNBASE;
	pte_t * ptable = kptable - KERNBASE;

	memset(pdir, 0, NPDENTRIES * sizeof(pte_t));

	for (pdir_idx = 0; pdir_idx < MAX_MEM / PTSIZE; pdir_idx ++) {
		pdir[pdir_idx] = (pde_t)ptable | PTE_P;
		ptable += NPDENTRIES;
	}
	asm volatile ("std;\
	 1: stosl;\
		subl %0, %%eax;\
		jge 1b" : : 
		"i"(PGSIZE), "a"((MAX_MEM - PGSIZE) | 0x7), "D"(ptable - 1));
	printk("%x\n", (int)kpdir);
	lcr3((uint32_t)kpdir - 0xf0000000);
	while(1);
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
