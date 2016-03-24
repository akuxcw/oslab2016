#include <types.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <kbd.h>
enum {KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS};

/* a-z */
const static int letter_code[] = {
	30, 48, 46, 32, 18, 33, 34, 35, 23, 36,
	37, 38, 50, 49, 24, 25, 16, 19, 31, 20,
	22, 47, 17, 45, 21, 44
};

#define NR_KEYS (sizeof(letter_code)/sizeof(int))

static int letter_pressed[26];

static int query_key(int index) {
	return letter_pressed[index];
}

extern int Jump, Vsx, Vy, Vsy, Vx, Vsx;
extern int restart;
int LastJump;
int Jump2;

void init_kbd() {
	memset(letter_pressed, 0, sizeof letter_pressed);
}

void press_key(int keys) {
//	printk("press %c\n", keys + 'a');
	letter_pressed[keys] = KEY_STATE_WAIT_RELEASE;
	switch(keys + 'a') {
		case 'w' : 
			if(Jump < 2) Vx = -Vsx/2, Jump ++, LastJump = time(), Jump2 = 0;
			break;
		case 'a' :
			Vy -= Vsy;
			break;
		case 'd' :
			Vy += Vsy;
			break;
		case 'r' :
			restart = true;
		default : ;
	}
}

void pressing_key(int keys) {
	switch(keys + 'a') {
		case 'w' :
			if(!Jump2 && time() - LastJump > 5) Vx += -Vsx/2, Jump2 ++;   
	}
}

void release_key(int keys) {
//	printk("release %c\n", keys + 'a');
	letter_pressed[keys] = KEY_STATE_EMPTY;
	switch(keys + 'a') {
		case 'w' : 
			break;
		case 'a' :
			Vy += Vsy;
			break;
		case 'd' :
			Vy -= Vsy;
			break;
		default : ;
	}

}

void process_kbd() {
	Update_kbd(letter_pressed, NR_KEYS);
//	cli();
	int i;
	bool flag = 0;
	for(i = 0; i < NR_KEYS; ++ i) {
		if(query_key(i) == KEY_STATE_PRESS) {
			flag = true;
			press_key(i);
		} else
		if(query_key(i) == KEY_STATE_RELEASE) {
			flag = true;
			release_key(i);
		} else
		if(query_key(i) == KEY_STATE_WAIT_RELEASE) {
			flag = true;
			pressing_key(i);
		}
	}
	if(!flag) Vy = 0;
//	sti();
}
