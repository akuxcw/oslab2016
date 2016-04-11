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
static PCB/* *last, */*current;
static uint32_t tot;

void set_tss_esp0(int);

void exec(TrapFrame *tf) {
//	printk("%x\n", tf->eip);
	if(current != NULL) {
		current->tf = *tf;
/*		last->tf.eax = tf->eax;
		last->tf.ebx = tf->ebx;
		last->tf.ecx = tf->ecx;
		last->tf.edx = tf->edx;
		last->tf.ebp = tf->ebp;
		last->tf.eip = tf->eip;
		last->tf.cs = tf->cs;
		last->tf.ss = tf->ss;
		last->tf.esp = tf->esp;
		last->tf.eflags = tf->eflags;
		last->tf.ds = tf->ds;
		last->tf.es = tf->es;
		last->tf.fs = tf->fs;
		last->tf.gs = tf->gs;
*/
	}
/*	ListHead *ptr, *ptr_;
	PCB *tmp;
	list_foreach_safe(ptr, ptr_, &Sleep) {
		tmp = list_entry(ptr, PCB, list);
		tmp->time --;
		printk("%%%%%%%%%%%%%% %x %x\n", tmp->time, tmp->pid);
		if(tmp->time <= 0) ready(tmp);
	}
	if(list_empty(&Ready)) {
		while(1);
	}*/
	current = list_entry(Ready.next, PCB, list);
//	printk("%x\n", current->pid);
//	assert(current == last);

//	last = current;
//	ready(current);
	set_tss_esp0((int)current->kstack + KSTACK_SIZE);
	lcr3(va2pa(current->pdir));
	asm volatile("movl %0, %%esp" : :"a"((int)&current->tf));

	asm volatile("pop %gs\n\t"
				 "pop %fs\n\t"
				 "pop %es\n\t"
				 "pop %ds\n\t"
				 "popa\n\t"
				 "addl $8, %esp\n\t"
				 "iret");
}

void ready(PCB *c) {
	list_del(&c->list);
	list_add_before(&Ready, &c->list);
}

void sleep(PCB *c, uint32_t t) {
//		printk("%%%%%%%%%%%%%%%x\n", c->pid);
	list_del(&c->list);
	c->time = t;
	list_add_before(&Sleep, &c->list);
/*	
	ListHead *ptr, *ptr_;
	PCB *tmp;
	list_foreach_safe(ptr, ptr_, &Sleep) {
		tmp = list_entry(ptr, PCB, list);
		tmp->time --;
		printk("%%%%%%%%%%%%%% %x %x\n", tmp->time, tmp->pid);
		if(tmp->time <= 0) ready(tmp);
	}
*/
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
