#include "vga.h"

const static InfoBlock *VbeInfo = (InfoBlock *)0xA01;
char *color_buffer;
uint32_t PADDR;
uint32_t PSIZE = 0x400000;

void init_palette() {
	color_buffer = (char *)VbeInfo->physbase;
	PADDR = VbeInfo->physbase;
}
