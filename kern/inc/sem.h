#ifndef KERN_INC_SEM_H
#define KERN_INC_SEM_H

#include <common.h>
#include <inc/list.h>

#define NR_SEM 256

typedef struct {
	int id, cnt;
	bool bin;
	ListHead wait_list;
	ListHead list;
} SEM;

#endif
