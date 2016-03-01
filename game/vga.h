typedef struct ModeInfoBlock {
	uint16_t attributes;
	uint8_t winA,winB;
	uint16_t granularity;
	uint16_t winsize;
	uint16_t segmentA, segmentB;
	uint32_t realFctPtr;
	uint16_t pitch; // bytes per scanline

	uint16_t Xres, Yres;
	uint8_t Wchar, Ychar, planes, bpp, banks;
	uint8_t memory_model, bank_size, image_pages;
	uint8_t reserved0;

	uint8_t red_mask, red_position;
	uint8_t green_mask, green_position;
	uint8_t blue_mask, blue_position;
	uint8_t rsv_mask, rsv_position;
	uint8_t directcolor_attributes;

	uint32_t physbase;  // your LFB (Linear Framebuffer) address ;)
	uint32_t reserved1;
	uint16_t reserved2;
} __attribute__((packed)) InfoBlock;

typedef struct _RGB{
	
		uint8_t R;
		uint8_t G;
		uint8_t B;
	
//	struct {
//		uint32_t val	:	24;
//	};
} __attribute__((packed)) RGB;

const InfoBlock *VbeInfo = (InfoBlock *)0xa01;
#define VGA_ADDR VbeInfo->physbase
#define V_ROW 600
#define V_COL 800
RGB * color_buffer;
#define color(x,y) color_buffer[x*V_COL+y]
#define toColor(x,y) {x.R=y&0xff,x.G=(y>>8)&0xff,x.B=(y>>16)&0xff;}
/*RGB operator = (int ) {
	this->R = x & 0xff;
	G = (x >> 8) & 0xff;
	B = (x >> 16) & 0xff;
	return *this;
}*/


