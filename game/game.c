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
int Property[700][3100];
int Block[20][100];
char BackGround[600*800*3];
int ans, goal;
int Gx, Gy, Gwidth;
int Xnow, Ynow, Width, Vy, Vsy, Vx, Vsx;
int Sky, Jump;
int g;
int Delta;

#define prop(i,j) Property[i+50][j+50]

void check_state();
void Set_property(int x, int y, int h, int l, int k);

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
//			  	toColor(color(i,j), Basic.arr[i * V_COL + j]);
				if(i >= Gx && i < Gx + Gwidth && j >= Gy && j < Gy + Gwidth) {
					if(!v[i][j]) ans ++, v[i][j] = true;
				}
			}
		if (ans == goal) goto START;
		check_state();
		do_move();
		process_kbd();
		process_video();
		while(Get_time() - time < Delta);
	}
}

void init_game() {
	/* Get framebuffer */
	color_buffer = (RGB *) VGA_ADDR;
	L2_color_buffer = (RGB *) (VGA_ADDR + 600 * 800 * 3);	
	memset(v, 0, sizeof v);	
	
	int i, j;
	for(i = 0; i < 700; ++ i) {
		for(j = 0; j < 900; ++ j) {
			prop(i,j) = 1;
		}
	}
	
	/* Set parameter */
	Gx = 0; Gy = 1500; Gwidth = 30;   //goal block
	ans = 0; goal = 30 * 30; 

	Xnow = 580, Ynow = 400, Width = 20;
	
	g = 6;
	Vy = 0;	Vsy = 50;
	Vx = 0;	Vsx = 120;

	Jump = 0;
	Delta = 2;

	for(i = 3; i < 20; i += 3)
	  	for(j = 0; j < 100; j += 3)
			Block[i][j] = GREEN;
	Block[0][50] = GOLDEN;

	/* Display background */
	Displayjpg(0, 0, &Basic);
	memcpy((void *)BackGround, (void *)L2_color_buffer, 600 * 800 * 3);	

	/* Set property */
	Set_property(0, 0, 700, 3100, SKY);
	Set_property(0, 0, 700, 10, GROUND);
	Set_property(600, 0, 100, 3100, GROUND);
	Set_property(0, 3150, 700, 50, GROUND);
	for(i = 0; i < 20; ++ i) 
		for(j = 0; j < 100; ++ j) {
			switch(Block[i][j]) {
				case GREEN :
					Set_property(i * 30, j * 30, 30, 30, GROUND);
					break;
				case GOLDEN :
					Set_property(i * 30, j * 30, 30, 30, SKY);
					break;
			}
		}

	Updata_vga();	
}

void do_move() {
	/*   x-ray  */
	int tmp = Xnow + Vx/10;
	if(Vx > 0) {
		if(prop(tmp + Width - 1,Ynow) == GROUND || prop(tmp + Width - 1,Ynow + Width - 1) == GROUND) {
			Jump = 0;
			while(prop(tmp + Width - 1,Ynow) == GROUND || prop(tmp + Width - 1,Ynow + Width - 1) == GROUND) tmp --;
		}
	} else if (Vx < 0) {
		if(prop(tmp,Ynow) ==  GROUND || prop(tmp,Ynow + Width - 1) == GROUND) {
			Vx = 0;
			while(prop(tmp,Ynow) == GROUND || prop(tmp,Ynow + Width - 1) == GROUND) tmp ++;
		}
	}
	Xnow = tmp;
	if (Sky || Jump) Vx += g; else Vx = 0;

	/*   y-ray  */
	tmp = Ynow + Vy/10;
	int d;
	if(Vy > 0) d = -1; else d = 1;
	while(prop(Xnow,tmp + Width - 1) == GROUND || prop(Xnow + Width - 1,tmp + Width - 1) == GROUND ||
				prop(Xnow,tmp) == GROUND || prop(Xnow + Width - 1,tmp) == GROUND) tmp +=d;
	Ynow = tmp;
}

void check_state() {
	if (prop(Xnow + Width,Ynow) == SKY && prop(Xnow + Width,Ynow + Width - 1) == SKY) Sky = 1;
	else Sky = 0;
}

void Set_property(int x, int y, int h, int l, int k) {
	int i, j;
	for(i = x; i < x + h; ++ i) 
	  	for(j = y; j < y + l; ++ j)
		  	prop(i,j) = k;
}
