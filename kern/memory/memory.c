#include <common.h>
#include <inc/memory.h>
#include <inc/mmu.h>
#include <inc/process.h>
#include <inc/disk.h>

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

uint32_t seg_alloc(uint32_t vaddr, PCB* current) {
	SegMan *tmp = Get_free_seg();
	uint32_t offset;
#ifdef USE_PAGE
	offset = 0;
#else
	offset = tmp->base - vaddr;
#endif
	set_segment(&gdt[tmp->cs], DPL_USER, SEG_EXECUTABLE | SEG_READABLE, offset, tmp->limit);
	set_segment(&gdt[tmp->ds], DPL_USER, SEG_WRITABLE, offset, tmp->limit);
	current->tf.cs = SELECTOR_USER(tmp->cs);
	current->tf.ds = current->tf.ss = current->tf.es =
	current->tf.fs = current->tf.gs = SELECTOR_USER(tmp->ds);
	return tmp->base;
}

uint32_t page_alloc(uint32_t vaddr, uint32_t size, PCB* current) {
	pde_t *pdir = current->pdir;
	uint32_t pdir_idx;
	for(pdir_idx = vaddr / PTSIZE; pdir_idx < (vaddr + size + PTSIZE) / PTSIZE; ++ pdir_idx) {
		if(pdir[pdir_idx] != 0x0) continue;
		pdir[pdir_idx] = Get_free_pg() | 0x7;
//		printk("%x %x\n", pdir[pdir_idx], *(int *)(pdir[pdir_idx] - 0x7));
	}
	uint32_t pa = ((*(int *)(current->pdir[vaddr/PTSIZE] - 0x7)) - 0x7) + (vaddr & ((1 << 22) - 1));
	return pa;
}

//typedef struct Proghdr Proghdr;
void readprog(uint32_t vaddr, uint32_t fsize, uint32_t msize, PCB *current, unsigned char * pa, uint32_t offset) {
//	offset -= (vaddr & ((1 << 22) - 1));
	uint32_t pdir_idx, paddr;
	unsigned char *i;
	for(pdir_idx = vaddr / PTSIZE; pdir_idx < (vaddr + msize + PTSIZE) / PTSIZE; ++ pdir_idx) {
		paddr = (*(int *)(current->pdir[pdir_idx] & (~0x7))) & (~0x7);
//		printk("%x %x %x\n", current->pdir[pdir_idx], paddr, offset + (pdir_idx - vaddr / PTSIZE) * PTSIZE);
		readseg((unsigned char *)(paddr), 
					PTSIZE, offset + (pdir_idx - vaddr / PTSIZE) * PTSIZE);
		if(PTSIZE > (int)fsize) {
			for(i = (unsigned char *)(paddr + fsize); i < (unsigned char *)(paddr + PTSIZE); *i ++ = 0);
			break;
		}
		fsize -= PTSIZE;
	}
	pdir_idx ++;
	for(; pdir_idx < (vaddr + msize + PTSIZE) / PTSIZE; ++ pdir_idx) {
		paddr = (*(int *)(current->pdir[pdir_idx] & (~0x7))) & (~0x7);
		for(i = (unsigned char *)paddr; i < (unsigned char *)(paddr + PTSIZE); *i ++ = 0);
	}

}
/*
uint32_t mm_malloc(uint32_t vaddr, uint32_t size, PCB* current) {
	if(!current->p) {
		SegMan *tmp = Get_free_seg();
		uint32_t offset;
#ifdef USE_PAGE
		offset = 0;
#else
		offset = tmp->base - vaddr;
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
	uint32_t pdir_idx;
	for(pdir_idx = vaddr / PTSIZE; pdir_idx < (vaddr + size + PTSIZE) / PTSIZE; ++ pdir_idx) {
		if(pdir[pdir_idx] != 0x0) continue;
		pdir[pdir_idx] = Get_free_pg() | 0x7;
//		readseg((const char *)(*(int *)(pdir[pdir_idx] & (~0x7))) & (~0x7), )
//		printk("%x\n", pdir[pdir_idx]);
	}
	uint32_t pa = ((*(int *)(current->pdir[vaddr/PTSIZE] - 0x7)) - 0x7) + (vaddr & ((1 << 22) - 1));
#else
	uint32_t pa = tmp->base;
#endif

	return pa;
}
*/
