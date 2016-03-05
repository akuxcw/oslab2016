#include <inc/string.h>
#include <inc/stdio.h>
#include "vga.h"

extern jpg Basic2;
extern jpg Basic;
extern jpg GreenBlock;
extern jpg GoldenBlock;

bool query_key(int);
void Delay(int t);

bool v[600][800];
int Property[700][900];
int ans, goal;
int Gx, Gy, Gwidth;
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
	for(i = 100; i < 600; i += 100) 
	  	for(j = 0; j < 800; j += 100)
			Displayjpg(i, j, &GreenBlock, 1);

	Gx = 10; Gy = 400; Gwidth = 30;
	Displayjpg(Gx, Gy, &GoldenBlock, 2);
	
	ans = 0; goal = 30 * 30; 
	Xnow = 580, Ynow = 10, Width = 20;
	
	g = 2;
	Vy = 0;	Vsy = 30;
	Vx = 0;	Vsx = 70;
	
	Jump = 0;
	Delta = 10;
	
}

void do_jump();
void do_move();
void check_state();

void game(){
START:
	init_game();
	int i, j;
	while(1) {
		for(i = Xnow; i < Xnow + Width; ++ i)
		  	for(j = Ynow; j < Ynow + Width; ++ j) {
			  	toColor(color(i,j), Basic2.arr[i * V_COL + j]);
				if(i >= Gx && i < Gx + Gwidth && j >= Gy && j < Gy + Gwidth) {
					if(!v[i][j]) ans ++, v[i][j] = true;
				}
			}
		if (ans == goal) goto START;
		check_state();
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
			while(Property[tmp + Width - 1][Ynow] == 1 || Property[tmp + Width - 1][Ynow + Width - 1] == 1) tmp --;
		}
	} else {
		if(Property[tmp][Ynow] == 1 || Property[tmp][Ynow + Width - 1] == 1) {
			Vx = - Vx;
			while(Property[tmp][Ynow] == 1 || Property[tmp][Ynow + Width - 1] == 1) tmp ++;
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

void check_state() {
	if (Property[Xnow + Width - 1][Ynow] == 0 && Property[Xnow + Width - 1][Ynow + Width - 1] == 0) Jump ++;
}
