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
PCB idle, *current = &idle;
static uint32_t tot;

void set_tss_esp0(int);

void schedule(TrapFrame *tf) {
//	printk("%x %x\n", tf->eip, (int)tf);
	current->tf = *tf;
	ListHead *ptr, *ptr_;
	PCB *tmp;
	list_foreach_safe(ptr, ptr_, &Sleep) {
		tmp = list_entry(ptr, PCB, list);
		tmp->time = tmp->time - 1;
//		printk("%%%%%%%%%%%%%% %x %x %x %x\n", tmp->time, (int)&tmp->time, (int)tf, (int)&current->tf);
		if(tmp->time <= 0) ready(tmp);
	}
	assert(!list_empty(&Ready));
	current = list_entry(Ready.next, PCB, list);
	
//	printk("%x\n", current->pid);

	ready(current);
	set_tss_esp0((int)current->kstack + KSTACK_SIZE);
	lcr3(va2pa(current->pdir));
	*tf = current->tf;
}

void ready(PCB *c) {
	list_del(&c->list);
	if(list_empty(&Ready)) list_add_before(&Ready, &c->list);
	else list_add_before(&idle.list, &c->list);
}

void sleep(PCB *c, uint32_t t) {
//		printk("%%%%%%%%%%%%%%%x\n", (int)&c->time);
	list_del(&c->list);
	c->time = t;
	list_add_before(&Sleep, &c->list);
}

void init_process() {
//	printk("init_process\n");
	tot = 0;
	list_init(&pcb_head);
	list_init(&Ready);
	list_init(&Sleep);
	list_init(&unused_pcb);
	for(int i = 0; i < NR_PCB; ++ i) {
		list_add_after(&unused_pcb, &pcb[i].list);
	}
	ready(&idle);
}

PCB *new_process() {
//	if(list_empty(&unused_pcb)) printk("Process full!\n");
	assert(!list_empty(&unused_pcb));
	PCB *new_pcb = list_entry(unused_pcb.next, PCB, list);
	++ tot;
	new_pcb->pid = tot;
	list_del(&new_pcb->list);
	list_add_after(&pcb_head, &new_pcb->list);
	return new_pcb;
}

void Free_process(PCB *val) {
	list_del(&val->list);
	list_add_after(&pcb_head, &val->list);
}

PCB * running_process() {
	return current;
}
