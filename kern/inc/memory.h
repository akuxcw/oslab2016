#ifndef KERN_INC_MEM
#define KERN_INC_MEM

#include <inc/list.h>
#include <inc/mmu.h>
#include <inc/process.h>

#define MAX_MEM 0x20000000
#define USER_STACK_TOP 0x8000000
#define USER_STACK_SIZE 0x400000

typedef struct {
	uint32_t base, limit, cs, ds;
	ListHead list;
} SegMan;

typedef struct {
	uint32_t addr, cnt;
	ListHead list;
} PgMan;

uint32_t Get_free_pg();
SegMan* Get_free_seg();
void set_segment(SegDesc *ptr, uint32_t pl, uint32_t type, uint32_t base, uint32_t limit); 
void Free_pg(int);
void use_pg(int);
uint32_t seg_alloc(uint32_t, PCB*);
uint32_t page_alloc(uint32_t, uint32_t, PCB*);

#endif
