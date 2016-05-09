#include <common.h>
#include <inc/sem.h>
#include <inc/list.h>
#include <inc/memory.h>

ListHead SemHead;
SEM Sem[NR_SEM];

int sem_open(int cnt, bool bin) {
	SEM *news = list_entry(SemHead.next, SEM, list);
	list_del(&news->list);
	news->cnt = cnt;
	news->bin = bin;
	return news->id;
}

void sem_close(int id) {
	list_add_before(&SemHead, &Sem[id].list);
}

void sem_wait(int id) {
		printk("%d\n", Sem[id].cnt);
	if(Sem[id].cnt > 0) {
		Sem[id].cnt --;
	} else {
		list_del(&current->list);
		list_add_before(&Sem[id].wait_list, &current->list);
	}
}

void sem_post(int id) {
	if(!list_empty(&Sem[id].wait_list)) {
		PCB * newp = list_entry(Sem[id].wait_list.next, PCB, list);
		ready(newp);
	} else {
		Sem[id].cnt ++;
		if(Sem[id].bin && Sem[id].cnt > 1) Sem[id].cnt = 1;
	}
}

void init_sem() {
	int i;
	list_init(&SemHead);
	for(i = 0; i < NR_SEM; ++ i) {
		Sem[i].id = i;
		list_init(&Sem[i].wait_list);
		list_add_before(&SemHead, &Sem[i].list);
	}
}

