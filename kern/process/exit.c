#include <common.h>
#include <inc/irq.h>
#include <inc/process.h>
#include <inc/x86.h>
#include <inc/string.h>
#include <inc/memory.h>

void exit(int x) {
	del_process(current);
	int i;
	for(i = 0; i < NPDENTRIES; ++ i) {
		if(current->pdir[i] & PTE_P) {
			Free_pg(PTE_ADDR(current->pdir[i]));
		}
	}
	memset(current, 0, sizeof current);
}
