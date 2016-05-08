#include <thread.h>
#include <stdio.h>

void c() {
	printf("thread\n");
	while(1);
}

int game_main() {
	printf("PC problem.\n");
	int id;
	thread_create(&id, &c, NULL);
	printf("main\n");
	while(1);
}
