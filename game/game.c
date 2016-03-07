#include <inc/string.h>
#include <inc/stdio.h>
#include "vga.h"

const static InfoBlock *VbeInfo = (InfoBlock *)0xa01;
enum {SKY, GROUND};

extern jpg Basic2;
extern jpg Basic;
extern jpg GreenBlock;
extern jpg GoldenBlock;

void Delay(int t);

bool v[600][800];
int Property[700][900];
int ans, goal;
int Gx, Gy, Gwidth;
int Xnow, Ynow, Width, Vy, Vsy, Vx, Vsx;
int Sky, Jump;
int g;
int Delta;

void check_state();

void init_game();
void process_kbd();
void process_move();
void process_video();

void game(){
START:
	init_game();
	int i, j;
	while(1) {
		for(i = Xnow; i < Xnow + Width; ++ i)
		  	for(j = Ynow; j < Ynow + Width; ++ j) {
			  	toColor(color(i,j), Basic.arr[i * V_COL + j]);
				if(i >= Gx && i < Gx + Gwidth && j >= Gy && j < Gy + Gwidth) {
					if(!v[i][j]) ans ++, v[i][j] = true;
				}
			}
		if (ans == goal) goto START;
		check_state();
		process_move();
		process_kbd();
		process_video();
		Delay(Delta);
	}
}

void init_game() {
	color_buffer = (RGB *) VGA_ADDR;
	memset(v, 0, sizeof v);	
	int i, j;
	for(i = 0; i < 700; ++ i) {
		for(j = 0; j < 900; ++ j) {
			Property[i][j] = 1;
		}
	}
	Displayjpg(0, 0, &Basic, SKY);
	for(i = 100; i < 600; i += 100) 
	  	for(j = 0; j < 800; j += 100)
			Displayjpg(i, j, &GreenBlock, GROUND);

	Gx = 10; Gy = 400; Gwidth = 30;
	Displayjpg(Gx, Gy, &GoldenBlock, SKY);
	
	ans = 0; goal = 30 * 30; 
	Xnow = 580, Ynow = 10, Width = 20;
	
	g = 2;
	Vy = 0;	Vsy = 30;
	Vx = 0;	Vsx = 70;
	
	Jump = 0;
	Delta = 20;
	
}

void process_move() {
	/*   x-ray  */
	int tmp = Xnow + Vx/10;
	if(Vx > 0) {
		if(Property[tmp + Width - 1][Ynow] == GROUND || Property[tmp + Width - 1][Ynow + Width - 1] == GROUND) {
			Jump = 0;
			while(Property[tmp + Width - 1][Ynow] == GROUND || Property[tmp + Width - 1][Ynow + Width - 1] == GROUND) tmp --;
		}
	} else if (Vx < 0) {
		if(Property[tmp][Ynow] ==  GROUND || Property[tmp][Ynow + Width - 1] == GROUND) {
			Vx = 0;
			while(Property[tmp][Ynow] == GROUND || Property[tmp][Ynow + Width - 1] == GROUND) tmp ++;
		}
	}
	Xnow = tmp;
	if (Sky || Jump) Vx += g; else Vx = 0;

	/*   y-ray  */
	tmp = Ynow + Vy/10;
	int d;
	if(Vy > 0) d = -1; else d = 1;
	while(Property[Xnow][tmp + Width - 1] == GROUND || Property[Xnow + Width - 1][tmp + Width - 1] == GROUND ||
				Property[Xnow][tmp] == GROUND || Property[Xnow + Width - 1][tmp] == GROUND) tmp +=d;
	Ynow = tmp;
}

void check_state() {
	if (Property[Xnow + Width][Ynow] == SKY && Property[Xnow + Width][Ynow + Width - 1] == SKY) Sky = 1;
	else Sky = 0;
}
