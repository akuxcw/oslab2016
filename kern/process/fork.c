#include <common.h>
#include <inc/irq.h>
#include <inc/process.h>
#include <inc/x86.h>
#include <inc/string.h>
extern PCB idle;

void set_user_page(PCB *);

int fork() {
	printk("---fork---\n");
	uint32_t i, pa, npa;
	PCB *current = running_process();
	PCB *newp = new_process();
	printk("%x %x\n", current->pid, newp->pid);
	newp->tf = current->tf;
	newp->tf.eax = 0;
	for(i = 0; i < KSTACK_SIZE; ++ i) {
		newp->kstack[i] = current->kstack[i];
	}
	lcr3(va2pa(idle.pdir));
	set_user_page(newp);
	for(i = 0; i < NPDENTRIES; ++ i) {
		if((current->pdir[i] & PTE_P) && !(newp->pdir[i] & PTE_P)) {
			newp->pdir[i] = Get_free_pg() | 0x7;
			pa = (*(int *)(current->pdir[i] & (~ 0x7))) & (~ 0x7);
			npa = (*(int *)(newp->pdir[i] & (~ 0x7))) & (~ 0x7);
			printk("%x %x %x %x\n", i, current->pdir[i], pa, npa);
			memcpy((void *)npa, (void *)pa, PTSIZE);
		}
	}
	printk("xxx\n");
	lcr3(va2pa(current->pdir));
	ready(newp);
	return newp->pid;
}
