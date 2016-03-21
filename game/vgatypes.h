#include <inc/types.h>
typedef struct _RGB{
	uint8_t R;
	uint8_t G;
	uint8_t B;
} __attribute__((packed)) RGB;

typedef struct jpgtype {
	int height;
	int length;
	int arr[600*800];
} jpg;

