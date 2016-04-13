#ifndef KERN_INC_MEM
#define KERN_INC_MEM

#include <inc/list.h>
#include <inc/mmu.h>

#define MAX_MEM 0x8000000

typedef struct {
	uint32_t base, limit, cs, ds;
	ListHead list;
} SegMan;

typedef struct {
	uint32_t addr;
	ListHead list;
} PgMan;

uint32_t Get_free_pg();
SegMan* Get_free_seg();
void set_segment(SegDesc *ptr, uint32_t pl, uint32_t type, uint32_t base, uint32_t limit); 

#endif
