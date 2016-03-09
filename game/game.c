#include <inc/string.h>
#include <inc/stdio.h>
#include "vga.h"

const static InfoBlock *VbeInfo = (InfoBlock *)0xa01;
enum {SKY, GROUND};
enum {EMPTY, GREEN, GOLDEN};

extern jpg Basic2;
extern jpg Basic;
extern jpg GreenBlock;
extern jpg GoldenBlock;

void Delay(int t);
int Get_time();

bool v[600][800];
int Property[700][900];
int Block[20][100];
int ans, goal;
int Gx, Gy, Gwidth;
int Xnow, Ynow, Width, Vy, Vsy, Vx, Vsx;
int Sky, Jump;
int g;
int Delta;

void check_state();

void init_game();
void do_move();
void process_kbd();
void process_video();

void game(){
START:
	init_game();
	int i, j;
	volatile int time;
	while(1) {
		time = Get_time();
		for(i = Xnow; i < Xnow + Width; ++ i)
		  	for(j = Ynow; j < Ynow + Width; ++ j) {
			  	toColor(color(i,j), Basic.arr[i * V_COL + j]);
				if(i >= Gx && i < Gx + Gwidth && j >= Gy && j < Gy + Gwidth) {
					if(!v[i][j]) ans ++, v[i][j] = true;
				}
			}
		if (ans == goal) goto START;
		check_state();
		do_move();
		process_kbd();
//		process_video();
		while(Get_time() - time < Delta);
	}
}

void init_game() {
	/* Get framebuffer */
	color_buffer = (RGB *) VGA_ADDR;
	L2_color_buffer = (RGB *) (VGA_ADDR - 0x1000000);	
	memset(v, 0, sizeof v);	
	
	/* Set property */
	int i, j;
	for(i = 0; i < 700; ++ i) {
		for(j = 0; j < 900; ++ j) {
			Property[i][j] = 1;
		}
	}
	
	/* Set parameter */
	Gx = 10; Gy = 400; Gwidth = 30;   //goal block
	ans = 0; goal = 30 * 30; 

	Xnow = 580, Ynow = 400, Width = 20;
	
	g = 6;
	Vy = 0;	Vsy = 50;
	Vx = 0;	Vsx = 120;

	Jump = 0;
	Delta = 3;

	for(i = 3; i < 20; i += 3)
	  	for(j = 0; j < 100; j += 3)
			Block[i][j] = GREEN;
	Block[0][50] = GOLDEN;

	/* Display picture */
	Displayjpg(0, 0, &Basic, SKY);
	
	for(i = 0; i < 20; ++ i) 
		for(j = 0; j < 100; ++ j) {
			if(400 + j * 30 - Ynow < 0 || 400 + j * 30 - Ynow > 770) continue;
			switch(Block[i][j]) {
				case GREEN :
					Displayjpg(i * 30, 400 + j * 30 - Ynow, &GreenBlock, GROUND);
					break;
				case GOLDEN :
					Displayjpg(i * 30, 400 + j * 30 - Ynow, &GoldenBlock, SKY);
					break;
			}
//			printk("%d %d\n", i, j);
		}
	Updata_vga();	
}

void do_move() {
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
