#include <thread.h>
#include <stdio.h>
#include <time.h>
#define n 3
#define NR_P 1
#define NR_C 1

int x;
sem_t mutex1, mutex2, full, empty;
int buf[n], in = 0, out = 0;

void p(void *);
void c(void *);

int game_main() {
	printf("PC problem.\n");
	int id;
	mutex1 = sem_open(1, 1);
	mutex2 = sem_open(1, 1);
	full = sem_open(0, 0);
	empty = sem_open(n, 0);
	printf("main\n");
	int i;
	for(i = 1; i <= NR_C; ++ i)
		thread_create(&id, &c, (void *)i);
	for(i = 1; i <= NR_P; ++ i) 
		thread_create(&id, &p, (void *)i);
	
	while(1);
}

void p(void *args) {
	int x = 0;
	while(1) {
		x ++;
		printf("1 %d\n", (int)args);
		sem_wait(empty);
		printf("2 %d\n", (int)args);
		sem_wait(mutex1);
		printf("3 %d produce %d\n", (int)args, x);
//		buf[in] = (int) args;
		buf[in] = x;
		printf("4 %d\n", (int)args);
		in = (in + 1) % n;
		printf("5 %d\n", (int)args);
		sleep(100);
		printf("6 %d\n", (int)args);
		sem_post(mutex1);
		printf("7 %d\n", (int)args);
		sem_post(full);
		printf("8 %d\n", (int)args);
	}
}

void c(void *args) {
	while(1) {
		sem_wait(full);
		sem_wait(mutex2);
		printf("%d %d\n", buf[out], (int)args);
		out = (out + 1) % n;
		sem_post(mutex2);
		sem_post(empty);
		sleep(200);
	}
}

