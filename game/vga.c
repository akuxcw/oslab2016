#include <inc/types.h>
#include "vga.h"

extern int jpg[600*800];
int printk(const char * fmt, ...);
int cons_getc(void);

void init_vga(){
	color_buffer = (RGB *) VGA_ADDR;
	int i, j;
	for(i = 0; i < V_COL * V_ROW; i ++) {
//		color_buffer[i].R = jpg[i] & 0xff;
//		color_buffer[i].G = (jpg[i] >> 8) & 0xff;
//		color_buffer[i].B = (jpg[i] >> 16) & 0xff;
//		color_buffer[i] = toColor(jpg[i]);
//		color_buffer[i] = {jpg[i] & 0xff,(jpg[i] >> 8) & 0xff,(jpg[i] >> 16) & 0xff};
		toColor(color_buffer[i], jpg[i]);
	}
	int x = 10, y = 10, k = 50, d = 5;
	while(1) {
		char ch = cons_getc();
		if (ch == 'w' && x > 0) x -= d;
		if (ch == 's' && x < V_ROW) x += d;
		if (ch == 'a' && y > 0) y -= d;
		if (ch == 'd' && x < V_COL) y += d;
		
		for(i = x; i < x + k; ++ i)
			for(j = y; j < y + k; ++ j)
				toColor(color(i,j),0xff00ff);
	}
//	printk("%x\n", VbeInfo->physbase);
//	printk("%x\n", VbeInfo);

}
