#include <thread.h>
#include <stdio.h>
#include <time.h>

int x;

void c() {
	x ++;
	sleep(10);
	int y = 2 * x;
	sleep(10);
	printf("thread %d %d\n", x, y);
	while(1);
}

int game_main() {
	printf("PC problem.\n");
	int id;
	thread_create(&id, &c, NULL);
	thread_create(&id, &c, NULL);
	thread_create(&id, &c, NULL);
	printf("main\n");
	while(1);
}
