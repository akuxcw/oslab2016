#include <inc/syscall.h>

void write_palette(char * L2_color_buffer) {
	syscall(SYS_palette, L2_color_buffer);
}
