#ifndef KERN_INC_SEM_H
#define KERN_INC_SEM_H

#include <common.h>
#include <inc/list.h>

#define NR_SEM 256

typedef struct {
	int id, cnt;
	bool bin;
	ListHead list;
} SEM;

int sem_open(int, bool);
void sem_close(int);
void sem_wait(int);
void sem_post(int);

#endif
