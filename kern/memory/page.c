#include <common.h>
#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/memory.h>
/*
__attribute__((__aligned__(PGSIZE)))
pde_t kpdir[MAX_MEM / PTSIZE];			// kernel page directory

__attribute__((__aligned__(PGSIZE)))
pte_t kptable[MAX_MEM / PGSIZE];		// kernel page tables
*/
PgMan page[MAX_MEM / PGSIZE];

ListHead free_pg;
ListHead used_pg;
/*
void set_kern_page() {
	uint32_t pdir_idx;

	memset(pdir, 0, NR_PDE * sizeof(PDE));

	for (pdir_idx = 0; pdir_idx < PHY_MEM / PT_SIZE; pdir_idx ++) {
		pdir[pdir_idx].val = make_pde(ptable);
		pdir[pdir_idx + KOFFSET / PT_SIZE].val = make_pde(ptable);

		ptable += NR_PTE;
	}
	asm volatile ("std;\
	 1: stosl;\
		subl %0, %%eax;\
		jge 1b" : : 
		"i"(PGSIZE), "a"((MAX_MEM - PGSIZE) | 0x7), "D"(ptable - 1));
}

*/
void init_page() {
	int i, tot = 0;
	list_init(&free_pg);
	list_init(&used_pg);
	for(i = 0x400000; i < MAX_MEM; i += PGSIZE) {
		page[tot].addr = i;
		list_add_before(&free_pg, &page[tot].list);
	}
}
