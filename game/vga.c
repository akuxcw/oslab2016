#include <inc/types.h>
#include "vga.h"

#define ROW 60
#define COL 80
unsigned char * color_buffer = (unsigned char*)0xA0000;
#define color(x,y) color_buffer[x*COL+y]

int printk(const char * fmt, ...);
//int a = 0;
InfoBlock vbeinfo;// = (InfoBlock *) 0x9000;
void init_vga(){
	int i;
	for(i = 0; i < COL*ROW; i ++) color_buffer[i] = 0xf;
	printk("%x\n", &vbeinfo);
}
