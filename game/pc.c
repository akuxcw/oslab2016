#include <thread.h>
#include <stdio.h>
#include <time.h>

int x;
sem_t sem;

void c(void *args) {
	sem_wait(sem);
	x ++;
//	sleep(10);
	int y = 2 * x;
	printf("thread %d %d %d\n", x, y, (int)args);
	sleep(100);
	sem_post(sem);
	while(1);
}

int game_main() {
	printf("PC problem.\n");
	int id;
	sem = sem_open(1, 1);
	printf("main\n");
	thread_create(&id, &c, (void *)1);
	thread_create(&id, &c, (void *)2);
	thread_create(&id, &c, (void *)3);
	
	sem_close(sem);
	while(1);
}
