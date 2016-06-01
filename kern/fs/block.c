#include <common.h>
#include <inc/fs.h>
#include <inc/string.h>

#define NR_BLOCK (MAXDISKSIZE/SECTSIZE)
typedef struct {
	uint32_t index;
	ListHead list;
} block_t;

block_t block[NR_BLOCK];

uint8_t zero[SECTSIZE];

ListHead block_head;
ListHead used_block;
void init_blocks(uint8_t * map, uint32_t size) {
	int i;
	memset(zero, 0, sizeof zero);
	list_init(&block_head);
	list_init(&used_block);
	printk("%d\n", size / 512);
	for(i = 0; i < size / 512; ++ i) {
		block[i].index = i;
		if(map[i / 8] & (1 << (i & 0x7))) list_add_before(&used_block, &block[i].list);
		else list_add_before(&block_head, &block[i].list);
	}
}

uint32_t get_new_block() {
	printk("wer\n");
	printk("%d\n", block_head);
	assert(!list_empty(&block_head));
	block_t *newb = list_entry(block_head.next, block_t, list);
	list_del(&newb->list);
	list_add_before(&used_block, &newb->list);
	printk("%d\n", newb->index);
	return newb->index;
}

void del_block(uint32_t i) {
	list_del(&block[i].list);
	list_add_before(&block_head, &block[i].list);
	ide_write(zero, i, 1);
}
