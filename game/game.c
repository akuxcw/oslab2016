#include <inc/types.h>
#include <inc/string.h>
#include <inc/stdio.h>
#include "data/jpg.h"

extern jpg Basic2;
extern jpg Basic;
extern jpg GreenBlock;

bool query_key(int);
void Delay(int t);

bool v[600][800];
int Property[700][900];
int ans, goal;
int Xnow, Ynow, Width, Vy, Vx, Vsx, Jump;

void init_game() {
	color_buffer = (RGB *) VGA_ADDR;
	memset(v, 0, sizeof v);	
	int i, j;
	for(i = 0; i < 700; ++ i) {
		for(j = 0; j < 900; ++ j) {
			Property[i][j] = 1;
		}
	}
	Displayjpg(0, 0, &Basic, 0);
	Displayjpg(200, 10, &GreenBlock, 1);
	ans = 0; goal = 600 * 800; 
	Xnow = 550, Ynow = 10, Width = 50, Vy = 1;
	Vx = 0;
	Vsx = 110;
	Jump = 0;
	
}

void do_jump();

void game(){
START:
	init_game();
	int i, j;
	while(1) {
		for(i = Xnow; i < Xnow + Width; ++ i)
		  	for(j = Ynow; j < Ynow + Width; ++ j) {
			  	toColor(color(i,j), Basic2.arr[i * V_COL + j]);
				if(i >= 0 && i < V_ROW && j >= 0 && j < V_COL) {
					if(!v[i][j]) ans ++, v[i][j] = true;
				}
			}
		if (ans == goal) goto START;
		do_jump();
		if (query_key('w' - 'a') && !Jump) Vx = -Vsx, Jump = true;
		if (query_key('a' - 'a') && Ynow > 0) Ynow -= Vy;
		if (query_key('d' - 'a') && Ynow < V_COL - Width) Ynow += Vy;
		for(i = Xnow; i < Xnow + Width; ++ i)
			for(j = Ynow; j < Ynow + Width; ++ j)
				toColor(color(i,j),0x00ff);
		Delay(5);
	}

}

void do_jump() {
	if (Jump) {
		int tmp = Xnow + Vx/10;
		if(Vx > 0) {
			if(Property[tmp + Width][Ynow] == 1 || Property[tmp + Width][Ynow + Width] == 1) {
				Jump = false;
				while(Property[tmp + Width][Ynow] == 1 || Property[tmp + Width][Ynow + Width] == 1) tmp --;
			}
		} else {
			if(Property[tmp][Ynow] == 1 || Property[tmp][Ynow + Width] == 1) {
				Vx = - Vx;
				while(Property[tmp][Ynow] == 1 || Property[tmp][Ynow + Width] == 1) tmp ++;
			}
		}
		Xnow = tmp;
		if (Jump) Vx += 1;
	}
}

