#ifndef LIB_KBD_H
#define LIB_KBD_H
#include <syscall.h>
static inline
void Update_kbd(int *kbd_state, int len) {
	syscall(SYS_kbd, kbd_state, len);
}
#endif
