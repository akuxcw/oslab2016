#include <inc/types.h>
#include "vga.h"

#define VGA_ADDR 0xfc000000
#define ROW 600
#define COL 800
unsigned char * color_buffer = (unsigned char*)VGA_ADDR;
#define color(x,y) color_buffer[x*COL+y]

int printk(const char * fmt, ...);
//int a = 0;
const InfoBlock *vbeinfo = (InfoBlock *)0xa01;
void init_vga(){
	int i;
	for(i = 0; i < 3*COL*ROW; i ++) {
		switch (i%3){
			case 0 : color_buffer[i] = 0xf;
				break;
			case 1 : color_buffer[i] = 0xf;
				break;
			case 2 : color_buffer[i] = 0x0;
				break;
		}
	}
	printk("%x\n", vbeinfo->physbase);
	printk("%x\n", vbeinfo);
}
