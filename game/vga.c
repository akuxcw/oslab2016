#include <inc/types.h>
#include "vga.h"

extern int jpg[600*800];
int printk(const char * fmt, ...);
//int a = 0;
void init_vga(){
	color_buffer = (RGB *) VGA_ADDR;
	int i;
	for(i = 0; i < V_COL * V_ROW; i ++) {
		color_buffer[i].R = jpg[i] & 0xff;
		color_buffer[i].G = (jpg[i] >> 8) & 0xff;
		color_buffer[i].B = (jpg[i] >> 16) & 0xff;
//		color_buffer[i].val = jpg[i];
	}
//	while(1) {
//		for()
//	}
	printk("%x\n", VbeInfo->physbase);
	printk("%x\n", VbeInfo);
}
