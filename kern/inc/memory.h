#ifndef KERN_INC_MEM
#define KERN_INC_MEM

#include<inc/list.h>

#define MAX_MEM 0x20000000

typedef struct {
	uint32_t base, limit, cs, ds;
	ListHead list;
} SegMan;

typedef struct {
	uint32_t addr;
	ListHead list;
} PgMan;

#endif
