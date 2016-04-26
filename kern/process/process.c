#include <common.h>
#include <inc/process.h>
#include <inc/list.h>
#include <inc/memory.h>
#include <inc/x86.h>
#include <inc/string.h>

PCB pcb[NR_PCB];
ListHead pcb_head;
//ListHead unused_pcb;
ListHead Ready;
ListHead Sleep;
PCB idle, *current = &idle;
static uint32_t tot;

void set_tss_esp0(int);

void schedule(TrapFrame *tf) {
//	printk("%x\n", current->pid);
	current->tf = tf;
	assert(!list_empty(&Ready));
	current = list_entry(Ready.next, PCB, list);
	if(current->time <= 0) {
		ready(current);
		current = list_entry(Ready.next, PCB, list);
		current->time = 1;
//	printk("%x\n", current->pid);

		set_tss_esp0((int)current->kstack + KSTACK_SIZE);
		lcr3(va2pa(current->pdir));
	}
//	*tf = current->tf;
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
//	list_init(&unused_pcb);
	for(int i = 0; i < NR_PCB; ++ i) {
//		printk("%x\n", (int)&pcb[i]);
		list_add_after(&pcb_head, &pcb[i].list);
	}
	ready(&idle);
}

PCB *new_process() {
//	if(list_empty(&unused_pcb)) printk("Process full!\n");
	assert(!list_empty(&pcb_head));
	PCB *new_pcb = list_entry(pcb_head.next, PCB, list);
	++ tot;
	new_pcb->pid = tot;
	list_del(&new_pcb->list);
//	list_add_after(&pcb_head, &new_pcb->list);
	memset(new_pcb->pdir, 0, sizeof new_pcb->pdir);
	return new_pcb;
}

void del_process(PCB *val) {
	list_del(&val->list);
	list_add_after(&pcb_head, &val->list);
}

