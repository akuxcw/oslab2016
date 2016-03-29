#include <common.h>
#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/memory.h>
#include <inc/string.h>
#include <inc/x86.h>
#include <inc/process.h>

__attribute__((__aligned__(PGSIZE)))
pde_t kpdir[NPDENTRIES];			// kernel page directory

__attribute__((__aligned__(PGSIZE)))
pte_t kptable[(2 * MAX_MEM + 0x1000000) / PGSIZE];		// kernel page tables

PgMan page[MAX_MEM / PGSIZE];

ListHead free_pg;
ListHead used_pg;

void set_kern_page() {
	uint32_t pdir_idx;

	pde_t * pdir = (pde_t *)((int)(kpdir) - KERNBASE);
	pte_t * ptable = (pte_t *)((int)(kptable) - KERNBASE);


	memset(pdir, 0, NPDENTRIES * sizeof(pte_t));

	for (pdir_idx = 0; pdir_idx < MAX_MEM / PTSIZE; pdir_idx ++) {
		pdir[pdir_idx] = (pde_t)ptable | 0x7;
		pdir[pdir_idx + KERNBASE / PTSIZE] = (pde_t)ptable | 0x7;
//		printk("%x\n", pdir[pdir_idx]);
		ptable += NPDENTRIES;
	}
	for(pdir_idx = 0xfc000000 / PTSIZE; pdir_idx < 0xfd000000 / PTSIZE; pdir_idx ++) {
		pdir[pdir_idx] = (pde_t)ptable | 0x7;
//		printk("%x\n", pdir[pdir_idx]);
		ptable += 1024;
	}
while(1);
	int32_t pframe_addr;
	ptable --;
	
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
	lcr3((uint32_t)pdir);
}

void set_user_page(PCB *current) {

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

uint32_t Get_free_pg() {
	assert(!list_empty(&free_pg));
	ListHead *new_pg = free_pg.next;
	list_del(new_pg);
	list_add_after(&used_pg, new_pg);
	return list_entry(new_pg, PgMan, list)->addr;
}

