#include "vgatypes.h"
#include <inc/string.h>
#define VGA_ADDR VbeInfo->physbase
#define V_ROW 600
#define V_COL 800
RGB * color_buffer;
RGB * L2_color_buffer;
#define color(x,y) L2_color_buffer[(x)*V_COL+(y)]
#define toColor(x,y) {x.R = (y)&0xff;\
					 x.G = ((y)>>8)&0xff;\
					 x.B = ((y)>>16)&0xff;}

extern int Property[700][900];

static inline void 
Displayjpg(int x, int y, jpg* graph, int p) {
	int i, j;
	for(i = 0; i < graph->height; ++ i)
		for(j = 0; j < graph->length; ++ j) {
			toColor(color(x + i, y + j), graph->arr[i * graph->length + j]);
			Property[x + i][y + j] = p;
		}
}

static inline void
Updata_vga() {
	memcpy((void*) color_buffer, (void*)L2_color_buffer, 600 * 800 * 3);
}
