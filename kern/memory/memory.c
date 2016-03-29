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
	SegMan *tmp = Get_free_seg();
	uint32_t offset;
#ifdef USE_PAGE
	offset = 0;
#else
	offset = tmp->base - vaddr;
#endif
	set_segment(&gdt[tmp->gdt], DPL_USER, type, offset, tmp->limit);
	return tmp;
	pde_t *pdir = current->pdir;
	pte_t *ptable = current->ptable;
	uint32_t pdir_idx, ptab_idx;
	for(pdir_idx = vaddr / PTSIZE; pdir_idx < (vaddr + size) / PTSIZE; ++ pdir_idx) {
		pdir[pdir_idx] = (pde_t)(ptable + pdir_idx * 1024) | 0x7;
	}
	for(ptab_idx = vaddr / PGSIZE; ptab_idx < (vaddr + size) / PGSIZE; ++ ptab_idx) {
		ptable[ptab_idx] = Get_free_pg();
	}
}
