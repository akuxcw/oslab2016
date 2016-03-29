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

SegMan *mm_malloc(uint32_t vaddr, uint32_t size, uint32_t type, PCB* current) {
	SegMan *tmp;
	if(type != 0) {
		tmp = Get_free_seg();
	}
/*	uint32_t offset;
#ifdef USE_PAGE
	offset = 0;
#else
	offset = tmp->base - vaddr;
#endif
//	if(type != 0)set_segment(&gdt[tmp->gdt], DPL_USER, type, offset, tmp->limit);
*/
#ifdef USE_PAGE
	pde_t *pdir = current->pdir;
	pte_t *ptable = current->ptable__;
	pte_t *i;
	uint32_t pdir_idx;
	for(pdir_idx = vaddr / PTSIZE; pdir_idx < (vaddr + size + PTSIZE) / PTSIZE; ++ pdir_idx) {
//		if(pdir[pdir_idx] != 0x0) continue;
		pdir[pdir_idx] = (pde_t)va2pa(ptable) | 0x7;
		ptable += 1024;
	}
	for(i = current->ptable__; i < ptable; ++ i) {
		*i = (pte_t)Get_free_pg() | 0x7;
//		printk("%x\n", *i);
	}
	tmp->base = (*current->ptable__) - 0x7;
	current->ptable__ = ptable;
#endif

	return tmp;
}
