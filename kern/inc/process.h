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
	int time;
	__attribute__((__aligned__(PGSIZE)))
	pde_t pdir[NPDENTRIES];
	ListHead list;
} __attribute__((packed)) PCB;

void init_process();
PCB* new_process();

void ready(PCB *);
void sleep(PCB *);
void exec(uint32_t);

#endif
