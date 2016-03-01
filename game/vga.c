#include <inc/types.h>
#include "vga.h"

extern int jpg[600*800];
int printk(const char * fmt, ...);
//int a = 0;
void init_vga(){
	color_buffer = (RGB *) VGA_ADDR;
	int i;
	for(i = 0; i < V_COL * V_ROW; i ++) {
/*		switch (i%3){
			case 0 : color_buffer[i] = jpg[i/3] & 0xff;
				break;
			case 1 : color_buffer[i] = (jpg[i/3] >> 8) & 0xff;
				break;
			case 2 : color_buffer[i] = (jpg[i/3] >> 16) & 0xff;
				break;
		}
*/
		color_buffer[i].val = jpg[i];
	}
//	while(1) {
//		for()
//	}
	printk("%x\n", VbeInfo->physbase);
	printk("%x\n", VbeInfo);
}
