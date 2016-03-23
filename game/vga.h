#include "vgatypes.h"
#include <string.h>
#include <palette.h>
#define V_ROW 600
#define V_COL 800
RGB * L2_color_buffer;
#define color(x,y) L2_color_buffer[(x)*V_COL+(y)]
#define toColor(x,y) {x.R = (y)&0xff;\
					 x.G = ((y)>>8)&0xff;\
					 x.B = ((y)>>16)&0xff;}

static inline void 
Displayjpg(int x, int y, jpg* graph) {
	int i, j;
	for(i = 0; i < graph->height; ++ i)
		for(j = 0; j < graph->length; ++ j) {
			toColor(color(x + i, y + j), graph->arr[i * graph->length + j]);
		}
}

static inline void
Updata_vga() {
	write_palette((char *)L2_color_buffer);
}
