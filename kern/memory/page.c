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
pte_t kptable[(MAX_MEM + 0x1000000) / PGSIZE];		// kernel page tables

__attribute__((__aligned__(PGSIZE)))
pte_t uptable[(MAX_MEM + 0x1000000) / PGSIZE];

PgMan page[MAX_MEM / PGSIZE];

ListHead free_pg;
ListHead used_pg;

extern uint32_t PADDR;
extern uint32_t PSIZE;

uint32_t Get_cr3() {
	return va2pa(kpdir);
}

void set_kern_page() {
	uint32_t pdir_idx;

	pde_t * pdir = (pde_t *)kpdir;
	pte_t * ptable = (pte_t *)kptable;

//	printk("%x %x %x %x\n", (int)pdir, (int)ptable, kpdir, kptable);
//	memset(pdir, 0, NPDENTRIES * sizeof(pte_t));

	for (pdir_idx = 0; pdir_idx < MAX_MEM / PTSIZE; pdir_idx ++) {
		pdir[pdir_idx] = (pde_t)va2pa(ptable) | PTE_P | PTE_W;
		pdir[pdir_idx + KERNBASE / PTSIZE] = (pde_t)va2pa(ptable) | PTE_P | PTE_W;
//		printk("%x\n", pdir[pdir_idx]);
		ptable += NPDENTRIES;
	}
//while(1);
	for(pdir_idx = PADDR / PTSIZE; pdir_idx < (PADDR + PSIZE) / PTSIZE; pdir_idx ++) {
		pdir[pdir_idx] = (pde_t)va2pa(ptable) | PTE_P | PTE_W;
//		printk("%x\n", pdir[pdir_idx]);
		ptable += NPDENTRIES;
	}
	int32_t pframe_addr;
	ptable --;
	
	for (pframe_addr = PADDR + PSIZE - PGSIZE; pframe_addr >= PADDR; pframe_addr -= PGSIZE) {
		*ptable = (pte_t)pframe_addr | PTE_P | PTE_W;
//		printk("%x %x\n", (int)ptable, *ptable);
		ptable --;
	}
	for (pframe_addr = MAX_MEM - PGSIZE; pframe_addr >= 0; pframe_addr -= PGSIZE) {
		*ptable = (pte_t)pframe_addr | PTE_P | PTE_W;
//		printk("%x %x\n", (int)ptable, *ptable);
		ptable --;
	}
	lcr3(va2pa(pdir));
}

void set_user_page(PCB *current) {
	pde_t * pdir = current->pdir;
	pte_t * ptable = (pte_t *)va2pa(uptable);//current->ptable;
	
	uint32_t pdir_idx;
	for (pdir_idx = 0; pdir_idx < 0x400000 / PTSIZE; pdir_idx ++) {
		pdir[pdir_idx] = (pde_t)(&ptable[pdir_idx * NPDENTRIES]) | PTE_P | PTE_W;
		pdir[pdir_idx + KERNBASE / PTSIZE] = (pde_t)(&ptable[pdir_idx * NPDENTRIES]) | PTE_P | PTE_W;
//		printk("%x\n", pdir[pdir_idx]);
	}
	
	for(pdir_idx = PADDR / PTSIZE; pdir_idx < (PADDR + PSIZE) / PTSIZE; pdir_idx ++) {
		pdir[pdir_idx] = (pde_t)(&ptable[MAX_MEM / PGSIZE + (pdir_idx - PADDR / PTSIZE) * NPDENTRIES]) | PTE_P | PTE_W;
//		printk("%x\n", pdir[pdir_idx]);
		ptable += NPDENTRIES;
	}
}

void init_page() {
	pte_t *ptable = (pte_t *)va2pa(uptable);
	
	int32_t pframe_addr;
	for (pframe_addr = 0 ; pframe_addr < 0x400000; pframe_addr += PGSIZE) {
		*ptable = (pte_t)pframe_addr | PTE_P | PTE_W;
//		printk("%x %x\n", (int)ptable, *ptable);
		ptable ++;
	}


	for (pframe_addr = 0x400000 ; pframe_addr < MAX_MEM; pframe_addr += PGSIZE) {
		*ptable = (pte_t)pframe_addr | PTE_P | PTE_W | PTE_U;
//		printk("%x %x\n", (int)ptable, *ptable);
		ptable ++;
	}

	for (pframe_addr = PADDR; pframe_addr < PADDR + PTSIZE; pframe_addr += PGSIZE) {
		*ptable = (pte_t)pframe_addr | PTE_P | PTE_W;
//		printk("%x %x\n", (int)ptable, *ptable);
		ptable ++;
	}
	
	ptable = (pte_t *)va2pa(uptable);
	int i, tot = 0;
	list_init(&free_pg);
	list_init(&used_pg);
	for(i = 0x400000; i < MAX_MEM; i += PTSIZE) {
		page[tot ++].addr = (int)&ptable[i / PGSIZE];
		printk("%x %x\n", page[tot - 1].addr, *(int *)page[tot-1].addr);
		list_add_after(&free_pg, &page[tot].list);
	}
	

}

uint32_t Get_free_pg() {
	assert(!list_empty(&free_pg));
	ListHead *new_pg = free_pg.next;
	list_del(new_pg);
	list_add_after(&used_pg, new_pg);
	return list_entry(new_pg, PgMan, list)->addr;
}

void Free_pg(PgMan *val) {
	list_del(&val->list);
	list_add_after(&free_pg, &val->list);
}
