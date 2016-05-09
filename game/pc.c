#include <thread.h>
#include <stdio.h>
#include <time.h>
#define n 2
#define NR_P 2
#define NR_C 1

int x;
sem_t mutex1, mutex2, full, empty;
int buf[n], in = 0, out = 0;

void p(void *);
void c(void *);

int game_main() {
	printf("PC problem.\n");
	int id;
	mutex1 = sem_open(1, 0);
	mutex2 = sem_open(1, 0);
	full = sem_open(0, 0);
	printf("full %d\n", full);
	empty = sem_open(n, 0);
	printf("main\n");
	int i;
	for(i = 1; i <= NR_C; ++ i) {
		thread_create(&id, &c, (void *)i);
		printf("C  %d\n", id);
	}
	for(i = 1; i <= NR_P; ++ i) {
		thread_create(&id, &p, (void *)i);
		printf("P  %d\n", id);
	}
//	sleep(-1);
	while(1)sleep(0x1ffffff);
	return 0;
}

void p(void *args) {
	int x = 0;
	while(1) {
		x ++;
		printf("p 1 %d\n", (int)args);
		sem_wait(empty);
		printf("p 2 %d\n", (int)args);
		sem_wait(mutex1);
		printf("p 3 %d produce %d\n", (int)args, x);
		buf[in] = x;
		printf("p 4 %d\n", (int)args);
		in = (in + 1) % n;
		printf("p 5 %d\n", (int)args);
		sem_post(mutex1);
		printf("p 6 %d\n", (int)args);
		sem_post(full);
		printf("p 7 %d\n", (int)args);
		sleep(100);
		printf("p 8 %d\n", (int)args);
	}
}

void c(void *args) {
	while(1) {
		printf("c 1 %d\n", (int)args);
		sem_wait(full);
		printf("c 2 %d\n", (int)args);
		sem_wait(mutex2);
		printf("c 3 consume %d %d\n", buf[out], (int)args);
		out = (out + 1) % n;
		printf("c 4 %d\n", (int)args);
		sem_post(mutex2);
		printf("c 5 %d\n", (int)args);
		sem_post(empty);
		printf("c 6 %d\n", (int)args);
		sleep(200);
		printf("c 7 %d\n", (int)args);
	}
}

