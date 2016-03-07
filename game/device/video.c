#include <inc/stdio.h>
#include "../vga.h"

enum {SKY, GROUND};

extern jpg Basic2;
extern jpg Basic;
extern jpg GreenBlock;
extern jpg GoldenBlock;
extern int Property[700][900];

void process_video() {
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

	Displayjpg(10, 400, &GoldenBlock, SKY);
	
}

