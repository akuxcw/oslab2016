#include <stdio.h>
#include <string.h>
#include "../vga.h"

enum {SKY, GROUND, DANGER, GOAL};
enum {EMPTY, GREEN, GOLDEN, RED};

extern jpg Basic2;
extern jpg Basic;
extern jpg GreenBlock;
extern jpg GoldenBlock;
extern jpg RedBlock;
extern int Property[700][900];
extern int Block[20][100];
extern char BackGround[600*800*3];

extern int Xnow, Ynow, Width;

void process_video() {
	int i, j;
	memcpy((void*)L2_color_buffer, (void*) BackGround, 600*800*3);
	for(i = Xnow; i < Xnow + Width; ++ i)
		for(j = 400; j < 400 + Width; ++ j)
			toColor(color(i,j),0x00ff);

	for(i = 0; i < 20; ++ i) 
		for(j = 0; j < 100; ++ j) {
			if(400 + j * 30 - Ynow < 0 || 400 + j * 30 - Ynow > 770) continue;
			switch(Block[i][j]) {
				case GREEN :
					Displayjpg(i * 30, 400 + j * 30 - Ynow, &GreenBlock);
					break;
				case GOLDEN :
					Displayjpg(i * 30, 400 + j * 30 - Ynow, &GoldenBlock);
					break;
				case RED :
					Displayjpg(i * 30, 400 + j * 30 - Ynow, &RedBlock);
			}
		}
	Updata_vga();
}

