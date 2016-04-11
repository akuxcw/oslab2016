#include <common.h>
#include <inc/process.h>
#include <inc/list.h>
#include <inc/memory.h>
#include <inc/x86.h>

PCB pcb[NR_PCB];
ListHead pcb_head;
ListHead unused_pcb;
ListHead Ready;
ListHead Sleep;
PCB *last;

void set_tss_esp0(int);

void exec(TrapFrame *tf) {
//	printk("%x\n", addr);
	if(last != NULL) {
		last->tf = *tf;
	}
	ListHead *ptr, *ptr_;
	PCB *tmp;
	list_foreach_safe(ptr, ptr_, &Sleep) {
		tmp = list_entry(ptr, PCB, list);
		tmp->time --;
		if(tmp->time <= 0) ready(tmp);
	}
	PCB *current = list_entry(Ready.next, PCB, list);
//	printk("%x\n", current->tf.eip);
	last = current;
	ready(current);
	set_tss_esp0((int)current->kstack + KSTACK_SIZE);
//	TrapFrame *tf = current->tf;	
	lcr3(va2pa(current->pdir));
	asm volatile("movl %0, %%esp" : :"a"((int)&current->tf));
	asm volatile("mov 64(%esp), %eax\n\t"
				 "movl %eax, %ds\n\t"
				 "movl %eax, %es\n\t"
				 "movl %eax, %fs\n\t"
				 "movl %eax, %gs\n\t");
	asm volatile("popa");
	asm volatile("addl $8, %esp");

	//while(1);
	asm volatile("iret");
}

void ready(PCB *c) {
	list_del(&c->list);
	list_add_before(&Ready, &c->list);
}

void sleep(PCB *c) {
	list_del(&c->list);
	list_add_before(&Sleep, &c->list);
}

void init_process() {
//	printk("init_process\n");
	list_init(&pcb_head);
	list_init(&Ready);
	list_init(&Sleep);
	list_init(&unused_pcb);
	for(int i = 0; i < NR_PCB; ++ i) {
		pcb[i].p = false;
		list_add_after(&unused_pcb, &pcb[i].list);
	}
}

PCB *new_process() {
//	if(list_empty(&unused_pcb)) printk("Process full!\n");
	assert(!list_empty(&unused_pcb));
	ListHead *new_pcb = unused_pcb.next;
	list_del(new_pcb);
	list_add_after(&pcb_head, new_pcb);
	return list_entry(new_pcb, PCB, list);
}

void Free_process(PCB *val) {
	list_del(&val->list);
	list_add_after(&pcb_head, &val->list);
}
