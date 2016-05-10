#ifndef KERN_INC_SEM_H
#define KERN_INC_SEM_H

#include <common.h>
#include <inc/list.h>

#define NR_SEM 256

typedef struct {
	int id, cnt;
	bool bin;
	char name[16];
	ListHead wait_list;
	ListHead list;
} SEM;

int sem_open(char *, int, bool);
void sem_close(int);
void sem_wait(int);
void sem_post(int);

#endif
