#include <inc/types.h>
#include <inc/x86.h>
#include <inc/stdio.h>

enum {KEY_STATE_EMPTY, KEY_STATE_WAIT_RELEASE, KEY_STATE_RELEASE, KEY_STATE_PRESS};

/* a-z */
const static int letter_code[] = {
	30, 48, 46, 32, 18, 33, 34, 35, 23, 36,
	37, 38, 50, 49, 24, 25, 16, 19, 31, 20,
	22, 47, 17, 45, 21, 44
};

#define NR_KEYS (sizeof(letter_code)/sizeof(int))

static int letter_pressed[26];

int query_key(int index) {
	return letter_pressed[index];
}

void kbd_event(int scan_code) {
//	printk("%x\n", scan_code);
	int i;
	bool flag;
	if(scan_code & 0x80) flag = false, scan_code -= 0x80; else flag = true;
	for (i = 0; i < 26; i ++) {
		if (letter_code[i] == scan_code) {
			if(!flag)letter_pressed[i] = KEY_STATE_RELEASE;
			if(flag && query_key(i) == KEY_STATE_EMPTY) 
				letter_pressed[i] = KEY_STATE_PRESS;
		}
	}

}

extern int Jump, Vsx, Vy, Vsy, Vx, Vsx;
extern int restart;

void press_key(int keys) {
//	printk("press %c\n", keys + 'a');
	letter_pressed[keys] = KEY_STATE_WAIT_RELEASE;
	switch(keys + 'a') {
		case 'w' : 
			if(Jump < 2) Vx = -Vsx, Jump ++;
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
	cli();
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
		}
	}
	if(!flag) Vx = 0;
	sti();
}
