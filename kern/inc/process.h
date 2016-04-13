#ifndef KERN_INC_PROCESS_H
#define KERN_INC_PRECESS_H

#include "irq.h"
#include "list.h"
#include <inc/memlayout.h>
#include <inc/memory.h>
#include <inc/mmu.h>
#include <inc/irq.h>

#define KSTACK_SIZE 4096
#define NR_PCB 16 

typedef struct PCB{
	uint8_t kstack[KSTACK_SIZE];
	TrapFrame tf;
	int time, pid;
	__attribute__((__aligned__(PGSIZE)))
	pde_t pdir[NPDENTRIES];
	ListHead list;
} __attribute__((packed)) PCB;

void init_process();
PCB* new_process();

void ready(PCB *);
void sleep(PCB *, uint32_t);
void schedule(TrapFrame *);
PCB *running_process();

#endif
