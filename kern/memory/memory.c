#include <common.h>
#include <inc/memory.h>
#include <inc/mmu.h>

extern SegDesc gdt[NR_SEGMENTS];

void init_seg();
void init_page();
SegMan* Get_free_seg();
void set_segment(SegDesc *ptr, uint32_t pl, uint32_t type, uint32_t base, uint32_t limit); 

void init_memory() {
	init_seg();
	init_page();
}

SegMan *mm_malloc(uint32_t vaddr, uint32_t size, uint32_t type) {
	SegMan *tmp = Get_free_seg();
	uint32_t offset;
#ifdef USE_PAGE
	offset = 0;
#else
	offset = tmp->base - vaddr;
#endif
	set_segment(&gdt[tmp->gdt], DPL_USER, type, offset, tmp->limit);
	return tmp;
}
