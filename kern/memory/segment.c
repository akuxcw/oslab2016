#include <common.h>
#include <inc/list.h>
#include <inc/mmu.h>
#include <inc/memory.h>

ListHead free_seg;
ListHead used_seg;
SegMan seg[MAX_MEM/SEG_SIZE];

void init_seg() {
	list_init(&free_seg);
	list_init(&used_seg);
	for(int i = 0; i < MAX_MEM/SEG_SIZE/2; i ++) {
#ifdef USE_PAGE
		seg[i].base = 0;
		seg[i].limit = 0xFFFFF;
#else
		seg[i].base = i * SEG_SIZE;
		seg[i].limit = 0x1FF;
#endif
		seg[i].cs = 2 * i + 1 + 3;
		seg[i].ds = 2 * i + 2 + 3;
		list_add_before(&free_seg, &seg[i].list);
	}
}

SegMan * Get_free_seg() {
	assert(!list_empty(&free_seg));
	ListHead *new_seg = free_seg.next;
	list_del(new_seg);
	list_add_after(&used_seg, new_seg);
	return list_entry(new_seg, SegMan, list);
}

