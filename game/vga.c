#define ROW 6000
#define COL 8000
unsigned char * color_buffer = (unsigned char*)0xA0000;
#define color(x,y) color_buffer[x*COL+y]
void init_vga(){
	int i;
	for(i = 0; i < 0x1000000; i ++) color_buffer[i] = 0x1;
}
