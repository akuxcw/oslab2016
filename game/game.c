#include <inc/types.h>
#include <inc/string.h>
#include <inc/stdio.h>
#include "data/jpg.h"

#define goal 600*800

extern jpg Basic2;
extern jpg Basic;
extern jpg GreenBlock;

int cons_getc(void);
bool query_key(int);
void Delay(int t);
bool v[600][800];

void game(){
	color_buffer = (RGB *) VGA_ADDR;
	int i, j;
START:
/*
	for(i = 0; i < V_COL * V_ROW; i ++) {
		toColor(color_buffer[i], Basic.arr[i]);
	}
*/
	Displayjpg(0, 0, &Basic);
	Displayjpg(10, 10, &GreenBlock);
	memset(v, 0, sizeof v);
	int ans = 0; 

	int x = 550, y = 10, k = 50, d = 1;
	int Vx = 0;
	int Vsx = 110;
	int jump = 0;
	while(1) {
		for(i = x; i < x + k; ++ i)
		  	for(j = y; j < y + k; ++ j) {
			  	toColor(color(i,j), Basic2.arr[i * V_COL + j]);
				if(i >= 0 && i < V_ROW && j >= 0 && j < V_COL) {
					if(!v[i][j]) ans ++, v[i][j] = true;
				}
			}
		if (ans == goal) goto START;
		if (jump) {
			x += Vx/10;
			if (Vx != Vsx) Vx += 1; else jump = false;
		}
		if (query_key('w' - 'a') && !jump) Vx = -Vsx, jump = true;
//		if (query_key('s' - 'a') && x < V_ROW - k) x += d;
		if (query_key('a' - 'a') && y > 0) y -= d;
		if (query_key('d' - 'a') && y < V_COL - k) y += d;
		for(i = x; i < x + k; ++ i)
			for(j = y; j < y + k; ++ j)
				toColor(color(i,j),0x00ff);
		Delay(5);
	}

}
