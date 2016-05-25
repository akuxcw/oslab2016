#include <string.h>
#include <stdio.h>
#include <time.h>
#include "vga.h"

enum {SKY, GROUND, DANGER, GOAL};
enum {EMPTY, GREEN, GOLDEN, RED};

extern jpg Basic2;
extern jpg Basic;
extern jpg GreenBlock;
extern jpg GoldenBlock;

char __buf[800*600*3];
bool v[600][800];
int Property[700][3100];
int Block[20][100];
char BackGround[600*800*3];
int ans, goal;
int Gx, Gy, Gwidth;
int Xnow, Ynow, Width, Vy, Vsy, Vx, Vsx;
int Jump;
int g;
int Delta;
int restart;
int state;

#define prop(i,j) Property[i+50][j+50]

void check_state();
void Set_property(int x, int y, int h, int l, int k);

void init_game();
void do_move();
void process_kbd();
void process_video();

void game(){
START:
//	while(1) printf("@");
	init_game();
	volatile int time_now;
	while(1) {
//		printf("@");
		time_now = time();
		do_move();
		process_kbd();
		process_video();
		check_state();
		switch(state) {
			case GOAL: goto RESTART;
			case DANGER : goto START;
		}
		int tmp = Delta - (time() - time_now);
		if (tmp > 0) sleep(tmp);
	}
RESTART:
	restart = 0;
	Displayjpg(0, 0, &Basic2);
	Updata_vga();
	while(1) {
		process_kbd();
		if(restart) goto START;
	}
}

void init_game() {
	/* Get framebuffer */
	L2_color_buffer = (RGB *)__buf;	
	memset(v, 0, sizeof v);	
	
	int i, j;
	
	/* Set parameter */
	Gx = 0; Gy = 1500; Gwidth = 30;   //goal block
	ans = 0; goal = 30 * 30; 

	Xnow = 580, Ynow = 400, Width = 20;
	
	g = 6;
	Vy = 0;	Vsy = 50;
	Vx = 0;	Vsx = 90;

	Jump = 0;
	Delta = 1;

	for(i = 3; i < 20; i += 3)
	  	for(j = 0; j < 100; j += 3)
			if((i+j)%2 == 0)Block[i][j] = GREEN; else Block[i][j] = RED;
	Block[18][1] = RED;
	Block[0][50] = GOLDEN;

	/* Display background */
	Displayjpg(0, 0, &Basic);
	memcpy((void *)BackGround, (void *)L2_color_buffer, 600 * 800 * 3);	

	/* Set property */
	Set_property(0, 0, 650, 3050, SKY);
	Set_property(-50, 0, 50, 3050, GROUND);		//Ceiling
	Set_property(0, 0, 650, 10, GROUND);		//Left wall
	Set_property(600, 0, 50, 3050, GROUND);		//Ground
	Set_property(0, 3000, 650, 50, GROUND);		//Right wall
	for(i = 0; i < 20; ++ i) 
		for(j = 0; j < 100; ++ j) {
			switch(Block[i][j]) {
				case GREEN :
					Set_property(i * 30, j * 30, 30, 30, GROUND);
					break;
				case GOLDEN :
					Set_property(i * 30, j * 30, 30, 30, GOAL);
					break;
				case RED : 
					Set_property(i * 30, j * 30, 30, 30, DANGER);
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
	if (state == SKY || Jump) Vx += g; else Vx = 0;

	/*   y-ray  */
	tmp = Ynow + Vy/10;
	int d;
	if(Vy > 0) d = -1; else d = 1;
	while(prop(Xnow,tmp + Width - 1) == GROUND || prop(Xnow + Width - 1,tmp + Width - 1) == GROUND ||
				prop(Xnow,tmp) == GROUND || prop(Xnow + Width - 1,tmp) == GROUND) tmp +=d;
	Ynow = tmp;
}

void check_state() {
	state = GROUND;
	if (prop(Xnow + Width,Ynow) == SKY && prop(Xnow + Width,Ynow + Width - 1)  != GROUND) state = SKY;
	int i, j;
	for(i = Xnow; i < Xnow + Width; ++ i) {
		for(j = Ynow; j < Ynow + Width; ++ j) {
			if(prop(i, j) == DANGER) state = DANGER;
			if(prop(i, j) == GOAL) state = GOAL;
		}
	}
}

void Set_property(int x, int y, int h, int l, int k) {
	int i, j;
	for(i = x; i < x + h; ++ i) 
	  	for(j = y; j < y + l; ++ j)
		  	prop(i,j) = k;
}
