#include <thread.h>
#include <stdio.h>
#include <time.h>

int x;

void c(void) {
	x ++;
//	sleep(10);
	int y = 2 * x;
	sleep(10);
	printf("thread %d %d\n", x, y);
	while(1);
}

void ping() {
	while(1) {
		printf("ping\n");
		sleep(100);
	}
}

void pang() {
	while(1) {
		printf("pang\n");
		sleep(200);
	}
}

int game_main() {
	printf("PC problem.\n");
	int id;
//	thread_create(&id, &c, NULL);
	thread_create(&id, &ping, NULL);
	thread_create(&id, &pang, NULL);
	printf("main\n");
	while(1);
}
