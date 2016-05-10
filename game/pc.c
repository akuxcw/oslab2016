#include <thread.h>
#include <stdio.h>
#include <time.h>
#include <process.h>
#define n 10
#define NR_P 0
#define NR_C 0

int x;
sem_t mutex1, mutex2, full, empty;
int buf[n], in = 0, out = 0;

void p(void *);
void c(void *);

void b();

void a() {
	int x;
	sleep(100);
	printf("a\n");
	thread_create(&x, &b, NULL);
	exit(0);
}

void b() {
	int x;
	sleep(200);
	printf("b\n");
	thread_create(&x, &a, NULL);
	exit(0);
}

int game_main() {
	int id;
	printf("PC problem.\n");
	thread_create(&id, a, NULL);
	sleep(-1);


	mutex1 = sem_open(1, 0);
	mutex2 = sem_open(1, 0);
	full = sem_open(0, 0);
//	printf("full %d\n", full);
	empty = sem_open(n, 0);
	printf("main\n");
	int i;
	for(i = 1; i <= NR_C; ++ i) {
		thread_create(&id, &c, (void *)i);
	//	printf("C  %d\n", id);
	}
	for(i = 1; i <= NR_P; ++ i) {
		thread_create(&id, &p, (void *)i);
	//	printf("P  %d\n", id);
	}
	while(1)sleep(0x1ffffff);
	return 0;
}

void p(void *args) {
	int x = 0;
	while(1) {
		x ++;
//		printf("p 1 %d\n", (int)args);
		sem_wait(empty);
//		printf("p 2 %d\n", (int)args);
		sem_wait(mutex1);
//		printf("p 3 %d\n", (int)args);
		sleep(200);
		printf("%d prod %d\n", (int)args, x);
		buf[in] = x;
//		printf("p 5 %d\n", (int)args);
		in = (in + 1) % n;
//		printf("p 6 %d\n", (int)args);
		sem_post(mutex1);
//		printf("p 7 %d\n", (int)args);
		sem_post(full);
//		printf("p 8 %d\n", (int)args);
	}
}

void c(void *args) {
	while(1) {
//		printf("c 1 %d\n", (int)args);
		sem_wait(full);
//		printf("c 2 %d\n", (int)args);
		sem_wait(mutex2);
		printf("%d consume %d\n", (int)args, buf[out]);
		out = (out + 1) % n;
//		printf("c 4 %d\n", (int)args);
		sem_post(mutex2);
//		printf("c 5 %d\n", (int)args);
		sem_post(empty);
//		printf("c 6 %d\n", (int)args);
		sleep(100);
//		printf("c 7 %d\n", (int)args);
	}
}

