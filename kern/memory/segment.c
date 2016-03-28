#include <common.h>
#include <inc/list.h>
#include <inc/mmu.h>
#include <inc/memory.h>

void set_segment(SegDesc *ptr, uint32_t pl, uint32_t type, uint32_t base, uint32_t limit); 

extern SegDesc gdt[NR_SEGMENTS];

ListHead free_seg;
ListHead used_seg;
SegMan seg[MAX_MEM/SEG_SIZE];

void init_memory() {
	list_init(&free_seg);
	list_init(&used_seg);
	for(int i = 1; i < MAX_MEM/SEG_SIZE; i ++) {
#ifdef USE_PAGE
		seg[i].base = 0;
		seg[i].limit = 0xFFFFF;
#else
		seg[i].base = i * SEG_SIZE;
		seg[i].limit = 0x1FF;
#endif
		seg[i].gdt = i + 3;
		list_add_before(&free_seg, &seg[i].list);
	}
}

SegMan *mm_malloc(uint32_t vaddr, uint32_t size, uint32_t type) {
	assert(!list_empty(&free_seg));
	ListHead *new_seg = free_seg.next;
	list_del(new_seg);
	list_add_after(&used_seg, new_seg);
	SegMan * tmp = list_entry(new_seg, SegMan, list);
	uint32_t offset;
#ifdef USE_PAGA
	offset = 0;
#else
	offset = tmp->base - vaddr;
#endif
	set_segment(&gdt[tmp->gdt], DPL_USER, type, offset, tmp->limit);
	return tmp;
}
