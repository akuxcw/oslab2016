#include <common.h>
#include <inc/fs.h>

#define NR_BLOCK (MAXDISKSIZE/SECTSIZE)
typedef struct {
	uint32_t index;
	ListHead list;
} block_t;

block_t block[NR_BLOCK];

ListHead block_head;
ListHead used_block;
void init_blocks(uint8_t * map, uint32_t size) {
	int i;
	list_init(&block_head);
	for(i = 0; i < size / 512; ++ i) {
		block[i].index = i;
		if(map[i / 8] & (1 << (i & 0x7))) list_add_before(&used_block, &block[i].list);
		else list_add_before(&block_head, &block[i].list);
	}
}

uint32_t get_new_block() {
	block_t *newb = list_entry(block_head.next, block_t, list);
	list_del(&newb->list);
	list_add_before(&used_block, &newb->list);
	return newb->index;
}

void del_block(int i) {
	list_del(&block[i].list);
	list_add_before(&block_head, &block[i].list);
}
