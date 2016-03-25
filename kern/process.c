#include <inc/process.h>
#include <inc/list.h>

PCB pcb[NR_PCB];
ListHead *pcb_head;
ListHead *unused_pcb;

void init_process() {
	list_init(pcb_head);
	list_init(unused_pcb);
	for(int i = 0; i < NR_PCB; ++ i) {
		list_add_after(&pcb[i].list, unused_pcb);
	}
}

PCB *new_process() {
	if(list_empty(unused_pcb)) printk("Process full!");
	ListHead *new_pcb = unused_pcb->next;
	list_del(new_pcb);
	list_add_after(new_pcb, pcb_head);
	return list_entry(new_pcb, PCB, list);
}
