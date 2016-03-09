#include <inc/stdio.h>
#include <inc/string.h>
#include "../vga.h"

enum {SKY, GROUND};
enum {EMPTY, GREEN, GOLDEN};

extern jpg Basic2;
extern jpg Basic;
extern jpg GreenBlock;
extern jpg GoldenBlock;
extern int Property[700][900];
extern int Block[20][100];
extern int * BackGround;

extern int Xnow, Ynow, Width;

void process_video() {
	int i, j;
//	Displayjpg(0, 0, &Basic, SKY);
//	memcpy((void*)L2_color_buffer, (void*) BackGround, 600*800*3);
//	memset(Property, 0, sizeof Property);
	for(i = Xnow; i < Xnow + Width; ++ i)
		for(j = 400; j < 400 + Width; ++ j)
			toColor(color(i,j),0x00ff);

	for(i = 0; i < 20; ++ i) 
		for(j = 0; j < 100; ++ j) {
			if(400 + j * 30 - Ynow < 0 || 400 + j * 30 - Ynow > 770) continue;
			switch(Block[i][j]) {
				case GREEN :
					Displayjpg(i * 30, 400 + j * 30 - Ynow, &GreenBlock, SKY);
					break;
				case GOLDEN :
					Displayjpg(i * 30, 400 + j * 30 - Ynow, &GoldenBlock, SKY);
					break;
			}
		}
	Updata_vga();
}

