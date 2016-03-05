#include "../vga.h"
typedef struct jpgtype {
	int height;
	int length;
	int arr[600*800];
} jpg;

static inline void 
Displayjpg(int x, int y, jpg* graph) {
	int i, j;
	for(i = 0; i < graph->height; ++ i)
		for(j = 0; j < graph->length; ++ j) {
			toColor(color(x + i, y + j), graph->arr[i * graph->length + j]);
		}
}
