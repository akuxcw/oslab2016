#include <common.h>
#include <inc/memory.h>
#include <inc/process.h>
#include <inc/string.h>
#include <inc/x86.h>

void thread_create(int *tid, int entry, int args) {
	PCB * newp = new_process();
	*tid = newp->pid;
	newp->tf = (TrapFrame *)((int)newp->kstack + (int)current->tf - (int)current->kstack);
	memcpy(newp->kstack, current->kstack, sizeof newp->kstack);
	memcpy(newp->pdir, current->pdir, sizeof newp->pdir);
	newp->tf->eip = entry;
//	newp->tf->esp -= 0x100;
//	newp->pdir[USER_STACK_TOP/PTSIZE - 1] = 0;
	page_alloc(USER_STACK_TOP - USER_STACK_SIZE, USER_STACK_SIZE, current);
	lcr3(va2pa(newp->pdir));
	printk("%d %x\n", args, newp->tf->esp);
	*(int *)(newp->tf->esp + 0x4) = args;
	lcr3(va2pa(current->pdir));
	ready(newp);
}
/*
void thread_join(int tid, int *res) {
	
}
*/
