#define ROW 200
#define COL 320
unsigned char * color_buffer = (unsigned char*)0xA0000;
#define color(x,y) color_buffer[x*COL+y]
void init_vga(){
//	color_buffer = 0xA0000;
	int i;
	for(i = 0; i < COL*ROW; i ++) color_buffer[i] = 0x1;
}
