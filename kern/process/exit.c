#include <common.h>
#include <inc/irq.h>
#include <inc/x86.h>
#include <inc/string.h>
#include <inc/memory.h>

void exit(int x) {
//	printk("exit %x\n", current->pid);
	del_process(current);
	int i;
	for(i = 0; i < NPDENTRIES; ++ i) {
		if((current->pdir[i] & PTE_P) && (current->pdir[i] & PTE_U)) {
//			printk("%x\n", PTE_ADDR(*(int *)PTE_ADDR(current->pdir[i])));
			Free_pg(PTE_ADDR(current->pdir[i]));
		}
	}
}
