#ifndef KERN_INC_PROCESS_H
#define KERN_INC_PRECESS_H

#include "irq.h"

#define KSTACK_SIZE 4096
#define NR_PCB 16

typedef struct PCB{
	TrapFrame tf;
	uint8_t kstack[KSTACK_SIZE];
	struct PCB *next;
} PCB;

void init_process();
PCB* new_process();

#endif
