#include <thread.h>
#include <stdio.h>

int x;

void c() {
	x ++;
	printf("thread %d\n", x);
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
