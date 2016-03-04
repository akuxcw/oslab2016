#include <inc/types.h>
#include "vga.h"

extern int jpg[600*800];
int printk(const char * fmt, ...);
int cons_getc(void);
bool query_key(int);
void Delay(int t);

void init_vga(){
	color_buffer = (RGB *) VGA_ADDR;
	int i, j;
	for(i = 0; i < V_COL * V_ROW; i ++) {
		toColor(color_buffer[i], jpg[i]);
	}
	int x = 10, y = 10, k = 50, d = 1;
	while(1) {
		for(i = x; i < x + k; ++ i)
		  	for(j = y; j < y + k; ++ j)
			  	toColor(color(i,j), jpg[i * V_COL + j]);
		if (query_key('w' - 'a') && x > 0) x -= d;
		if (query_key('s' - 'a') && x < V_ROW) x += d;
		if (query_key('a' - 'a') && y > 0) y -= d;
		if (query_key('d' - 'a') && x < V_COL) y += d;
		for(i = x; i < x + k; ++ i)
			for(j = y; j < y + k; ++ j)
				toColor(color(i,j),0x00ff);
		Delay(1);
	}

}
