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
		seg[i].base = i * SEG_SIZE;
		seg[i].limit = 0x1fff;
		seg[i].gdt = i + 3;
		list_add_after(&free_seg, &seg[i].list);
	}
}

SegMan *mm_malloc(uint32_t vaddr, uint32_t size, uint32_t type) {
	if(list_empty(&free_seg))printk("Memory full!\n");
	ListHead *new_seg = free_seg.next;
	list_del(new_seg);
	list_add_after(&used_seg, new_seg);
	SegMan * tmp = list_entry(new_seg, SegMan, list);
	set_segment(&gdt[tmp->gdt], DPL_USER, type, tmp->base - vaddr, tmp->limit);
	return tmp;
}
