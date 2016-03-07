#include <inc/types.h>
#include <inc/x86.h>
#include <inc/stdio.h>

/* a-z对应的键盘扫描码 */
static int letter_code[] = {
	30, 48, 46, 32, 18, 33, 34, 35, 23, 36,
	37, 38, 50, 49, 24, 25, 16, 19, 31, 20,
	22, 47, 17, 45, 21, 44
};
/* 对应键按下的标志位 */
static bool letter_pressed[26];

void
press_key(int scan_code) {
	int i;
	bool flag;
	if(scan_code & 0x80) flag = false, scan_code -= 0x80; else flag = true;
	for (i = 0; i < 26; i ++) {
		if (letter_code[i] == scan_code) {
			letter_pressed[i] = flag;
		}
	}
}

void
release_key(int index) {
	letter_pressed[index] = false;
}

bool
query_key(int index) {
	return letter_pressed[index];
}

void
kbd_event(int code) {
	press_key(code);
}

extern int Jump, Vsx, Vy, Vsy, Vx, Vsx;

void process_kbd() {
	if (query_key('w' - 'a') && Jump < 2) Vx = -Vsx, Jump ++;
	if (query_key('a' - 'a')) Vy = -Vsy;
	if (query_key('d' - 'a')) Vy = +Vsy;
}
