#include <inc/types.h>
#include "vga.h"

#define ROW 6000
#define COL 8000
unsigned char * color_buffer = (unsigned char*)0xA0000;
#define color(x,y) color_buffer[x*COL+y]

int printk(const char *fmt, ...);
//int a = 0;
//InfoBlock *vbeinfo = ;
void init_vga(){
	int i;
	for(i = 0; i < COL*ROW; i ++) color_buffer[i] = 0xf;
	printk("%x\n", &i);
}
