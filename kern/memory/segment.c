#include <common.h>
#include <inc/list.h>
#include <inc/mmu.h>

void init_memory() {
	for(int i = 0x2000000; i < 0x20000000; i += 0x2000000) {
		
	}
}

uint32_t mm_malloc(uint32_t vaddr, uint32_t size, uint32_t type) {
	return 0;
}
