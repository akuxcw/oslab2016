#ifndef KERN_INC_PROCESS_H
#define KERN_INC_PRECESS_H

#include "irq.h"
#include "list.h"
#include <inc/memlayout.h>
#include <inc/memory.h>
#include <inc/mmu.h>

#define KSTACK_SIZE 4096
#define NR_PCB 2

typedef struct PCB{
	TrapFrame tf;
	uint8_t kstack[KSTACK_SIZE];
	pde_t pdir[NPDENTRIES];
	pte_t ptable[0x2000000 / PGSIZE];
	ListHead list;
} __attribute__((packed)) PCB;

void init_process();
PCB* new_process();

#endif
