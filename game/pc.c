#include <thread.h>
#include <stdio.h>
#include <time.h>
#define n 10

int x;
sem_t mutex1, mutex2, full, empty;
int buf[n], in = 0, out = 0;
void p(void *args) {
	while(1) {
		sem_wait(empty);
		sem_wait(mutex1);
		buf[in] = (int) args;
		in = (in + 1) % n;
		sleep(100);
		sem_post(mutex1);
		sem_post(full);
	}
}

void c(void *args) {
	while(1) {
		sem_wait(full);
		sem_wait(mutex2);
		printf("%d %d\n", buf[out], (int)args);
		out = (out + 1) % n;
		sleep(200);
		sem_post(mutex2);
		sem_post(empty);
	}
}

int game_main() {
	printf("PC problem.\n");
	int id;
	mutex1 = sem_open(1, 1);
	mutex2 = sem_open(1, 1);
	full = sem_open(0, 0);
	empty = sem_open(n, 0);
	printf("main\n");
	thread_create(&id, &c, (void *)1);
	thread_create(&id, &c, (void *)2);
	thread_create(&id, &c, (void *)3);
	
	while(1);
}
