#include <common.h>
#include <inc/irq.h>
#include <inc/process.h>
#include <inc/x86.h>
#include <inc/string.h>
#include <inc/memory.h>

void exit(int x) {
//	printk("exit %d\n", current->pid);
	del_process(current);
	int i;
	for(i = 0; i < NPDENTRIES; ++ i) {
		if(current->pdir[i] & PTE_P && current->pdir[i] & PTE_U) {
			Free_pg(PTE_ADDR(current->pdir[i]));
		}
//		current->pdir[i] = 0;
	}
//	printk("%x %x\n", sizeof current->pdir, sizeof a);
	memset(current, 0, sizeof current);
}
