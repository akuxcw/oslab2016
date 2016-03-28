#ifndef KERN_INC_MEM
#define KREN_INC_MEM

#include<inc/list.h>

#define MAX_MEM 0x8000000

typedef struct {
	uint32_t base, limit, gdt;
	ListHead list;
} SegMan;

typedef struct {
	uint32_t addr;
	ListHead list;
} PgMan;

#endif
