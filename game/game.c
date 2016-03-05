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
int Xnow, Ynow, Width, Vy, Vsy, Vx, Vsx, Jump;
int g;
int Delta;

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
	Displayjpg(230, 10, &GreenBlock, 1);
	
	ans = 0; goal = 600 * 800; 
	Xnow = 550, Ynow = 10, Width = 50;
	
	g = 3;
	Vy = 0;	Vsy = 30;
	Vx = 0;	Vsx = 150;
	
	Jump = 0;
	Delta = 10;
	
}

void do_jump();
void do_move();

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
		if(Jump) do_jump();
		if(Vy != 0) do_move();
		if (query_key('w' - 'a') && Jump < 2) Vx = -Vsx, Jump ++;
		if (query_key('a' - 'a')) Vy = -Vsy;
		if (query_key('d' - 'a')) Vy = +Vsy;
		for(i = Xnow; i < Xnow + Width; ++ i)
			for(j = Ynow; j < Ynow + Width; ++ j)
				toColor(color(i,j),0x00ff);
		Delay(Delta);
	}

}

void do_jump() {
	int tmp = Xnow + Vx/10;
	if(Vx > 0) {
		if(Property[tmp + Width - 1][Ynow] == 1 || Property[tmp + Width - 1][Ynow + Width - 1] == 1) {
			Jump = false;
//			tmp = Xnow;
			while(Property[tmp + Width - 1][Ynow] == 1 || Property[tmp + Width - 1][Ynow + Width - 1] == 1) tmp --;
		}
	} else {
		if(Property[tmp][Ynow] == 1 || Property[tmp][Ynow + Width - 1] == 1) {
			Vx = - Vx;
			while(Property[tmp][Ynow] == 1 || Property[tmp][Ynow + Width - 1] == 1) tmp ++;
//			tmp = Xnow;
		}
	}
	Xnow = tmp;
	if (Jump) Vx += g;
}

void do_move() {
	int tmp = Ynow + Vy/10;
	int d;
	if(Vy > 0) d = -1; else d = 1;
	while(Property[Xnow][tmp + Width - 1] == 1 || Property[Xnow + Width - 1][tmp + Width - 1] == 1 ||
				Property[Xnow][tmp] == 1 || Property[Xnow + Width - 1][tmp] == 1) tmp +=d;
	Ynow = tmp;
	Vy = 0;
}

