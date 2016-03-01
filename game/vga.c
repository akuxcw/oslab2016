#include <inc/types.h>
#include "vga.h"

#define VGA_ADDR 0xfc000000
#define ROW 600
#define COL 800
unsigned char * color_buffer = (unsigned char*)VGA_ADDR;
#define color(x,y) color_buffer[x*COL+y]

extern int jpg[600*800];
int printk(const char * fmt, ...);
//int a = 0;
const InfoBlock *vbeinfo = (InfoBlock *)0xa01;
void init_vga(){
	int i;
	for(i = 0; i < 3*COL*ROW; i ++) {
		switch (i%3){
			case 0 : color_buffer[i] = jpg[i/3] & 0xff;
				break;
			case 1 : color_buffer[i] = (jpg[i/3] >> 8) & 0xff;
				break;
			case 2 : color_buffer[i] = (jpg[i/3] >> 16) & 0xff;
				break;
		}
	}
	printk("%x\n", vbeinfo->physbase);
	printk("%x\n", vbeinfo);
}
