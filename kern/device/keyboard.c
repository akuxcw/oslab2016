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

int Get_time();

int query_key(int index) {
	return letter_pressed[index];
}

void do_kbd(int scan_code) {
//	printk("%x\n", scan_code);
	int i;
	bool flag;
	if(scan_code & 0x80) flag = false, scan_code -= 0x80; else flag = true;
	for (i = 0; i < 26; i ++) {
		if (letter_code[i] == scan_code) {
			if(!flag)letter_pressed[i] = KEY_STATE_RELEASE;
			else letter_pressed[i] = KEY_STATE_PRESS;
		}
	}

}


