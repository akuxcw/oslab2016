#ifndef KERN_INC_PROCESS_H
#define KERN_INC_PRECESS_H

#include "irq.h"
#include "list.h"
#include <inc/memlayout.h>

#define KSTACK_SIZE 4096
#define NR_PCB 16

typedef struct PCB{
	TrapFrame tf;
	uint8_t kstack[KSTACK_SIZE];
	pde_t pdir[NPDENTRIES];
	ListHead list;
} PCB;

void init_process();
PCB* new_process();

#endif
