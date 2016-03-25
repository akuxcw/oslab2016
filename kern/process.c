#include <inc/process.h>

PCB pcb[NR_PCB];
PCB *pcb_head;
PCB *unused_pcb = pcb;

void init_process() {
	int i;
	PCB *tmp = unused_pcb;
	for(i = 1; i < NR_PCB; ++ i) {
		tmp->next = &pcb[i];
		tmp = tmp->next;
	}
}

PCB *new_process() {
	PCB *tmp = unused_pcb;
	unused_pcb = unused_pcb->next;
	tmp->next = NULL;
	if(pcb_head == NULL) pcb_head = tmp; else {
		PCB *tmp2 = pcb_head;
		while(tmp2->next != NULL) tmp2 = tmp2->next;
		tmp2->next = tmp;
	}
	return tmp;
}
