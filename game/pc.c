#include <thread.h>
#include <stdio.h>
#include <time.h>

int x;

void c(void *args) {
	x ++;
//	sleep(10);
	int y = 2 * x;
	sleep(10);
	printf("thread %d %d %d\n", x, y, (int)args);
	while(1);
}

void ping() {
	int x = 0;
	while(1) {
		x ++;
		printf("ping %d\n", x);
		sleep(100);
	}
}

void pang() {
	int x = 0;
	while(1) {
		x ++;
		printf("pang %d\n", x);
		sleep(200);
	}
}

int game_main() {
	printf("PC problem.\n");
	int id;
	thread_create(&id, &c, (void *)123);
//	thread_create(&id, &ping, NULL);
//	thread_create(&id, &pang, NULL);
	printf("main\n");
	while(1);
}
