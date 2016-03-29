#include <common.h>
#include <inc/memory.h>
#include <inc/mmu.h>
#include <inc/process.h>

extern SegDesc gdt[NR_SEGMENTS];

void init_seg();
void init_page();
SegMan* Get_free_seg();
uint32_t Get_free_pg();
void set_segment(SegDesc *ptr, uint32_t pl, uint32_t type, uint32_t base, uint32_t limit); 

void init_memory() {
	init_seg();
	init_page();
}

uint32_t mm_malloc(uint32_t vaddr, uint32_t size, PCB* current) {
	uint32_t pa;
	if(!current->p) {
		SegMan *tmp = Get_free_seg();
		uint32_t offset;
#ifdef USE_PAGE
		offset = 0;
#else
		offset = tmp->base - vaddr;
		pa = tmp->base;
#endif
		set_segment(&gdt[tmp->cs], DPL_USER, SEG_EXECUTABLE | SEG_READABLE, offset, tmp->limit);
		set_segment(&gdt[tmp->ds], DPL_USER, SEG_WRITABLE, offset, tmp->limit);
		//set_segment(&gdt[tmp->cs], DPL_USER, type, offset, tmp->limit);
		current->tf.cs = SELECTOR_USER(tmp->cs);
		current->tf.ds = current->tf.ss = current->tf.es =
		current->tf.fs = current->tf.gs = SELECTOR_USER(tmp->ds);
		current->p = true;
	}
#ifdef USE_PAGE
	pde_t *pdir = current->pdir;
	pte_t *ptable = current->ptable__;
	pte_t *i;
	uint32_t pdir_idx;
	for(pdir_idx = vaddr / PTSIZE; pdir_idx < (vaddr + size + PTSIZE) / PTSIZE; ++ pdir_idx) {
		if(pdir[pdir_idx] != 0x0) continue;
		pdir[pdir_idx] = (pde_t)va2pa(ptable) | 0x7;
		ptable += 1024;
	}
	for(i = current->ptable__; i < ptable; ++ i) {
		*i = (pte_t)Get_free_pg() | 0x7;
//		printk("%x\n", *i);
	}
	pa = (*current->ptable__) - 0x7;
	current->ptable__ = ptable;
#endif

	return pa;
}
