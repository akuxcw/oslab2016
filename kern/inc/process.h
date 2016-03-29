#ifndef KERN_INC_PROCESS_H
#define KERN_INC_PRECESS_H

#include "irq.h"
#include "list.h"
#include <inc/memlayout.h>
#include <inc/memory.h>
#include <inc/mmu.h>

#define KSTACK_SIZE 4096
#define NR_PCB 16 

typedef struct PCB{
	TrapFrame tf;
	uint8_t kstack[KSTACK_SIZE];
	bool p;
	pte_t * ptable__;
	__attribute__((__aligned__(PGSIZE)))
	pde_t pdir[NPDENTRIES];
	__attribute__((__aligned__(PGSIZE)))
	pte_t ptable[0x3000000 / PGSIZE];
	ListHead list;
} __attribute__((packed)) PCB;

void init_process();
PCB* new_process();

#endif
