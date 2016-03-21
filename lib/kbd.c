#include <inc/syscall.h>

void Update_kbd(int *kbd_state, int len) {
	syscall(SYS_kbd, kbd_state, len);
}
