#include <common.h>
#include <inc/irq.h>
#include <inc/memory.h>
#include <inc/x86.h>
#include <inc/string.h>
void set_user_page(PCB *);

int fork() {
//	printk("---fork---\n");
	uint32_t i, pa, npa;
//	printk("%x\n", running_process()->pdir[0x1f]);
	PCB *newp = new_process();
//	printk("%x %x\n", current->pid, newp->pid);
	newp->tf = (TrapFrame *)((int)newp->kstack + (int)current->tf - (int)current->kstack);
/*	for(i = 0; i < KSTACK_SIZE; ++ i) {
		newp->kstack[i] = current->kstack[i];
	}
*/	
	memcpy(newp->kstack, current->kstack, sizeof newp->kstack);
	newp->tf->eax = 0;
	lcr3(va2pa(idle.pdir));
	set_user_page(newp);
	for(i = 0; i < NPDENTRIES; ++ i) {
		if((current->pdir[i] & PTE_P) && !(newp->pdir[i] & PTE_P)) {
			newp->pdir[i] = Get_free_pg() | (current->pdir[i] & 0xfff);
			pa = PTE_ADDR(*(int *)PTE_ADDR(current->pdir[i]));
			npa = PTE_ADDR(*(int *)PTE_ADDR(newp->pdir[i]));
//			printk("%x %x %x\n", i, pa, npa);
			memcpy((void *)npa, (void *)pa, PTSIZE);
		}
	}
//	printk("xxx\n");
	lcr3(va2pa(current->pdir));
	ready(newp);
	return newp->pid;
}
