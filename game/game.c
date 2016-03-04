#include <inc/types.h>
#include <inc/stdio.h>
#include "vga.h"

extern int jpg[600*800];
int cons_getc(void);
bool query_key(int);
void Delay(int t);

void game(){
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
		if (query_key('s' - 'a') && x < V_ROW - k) x += d;
		if (query_key('a' - 'a') && y > 0) y -= d;
		if (query_key('d' - 'a') && y < V_COL - k) y += d;
		for(i = x; i < x + k; ++ i)
			for(j = y; j < y + k; ++ j)
				toColor(color(i,j),0x00ff);
		Delay(5);
	}

}
