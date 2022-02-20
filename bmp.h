#ifndef __BMP_H__
#define __BMP_H__
#include <stdlib.h>
#include <stdio.h>

//define pixelformat of windows bitmap, notice the unusual ordering of colors
struct pixel{
	unsigned char B;
	unsigned char G;
	unsigned char R;

#ifdef PXPADDING
	unsigned char padding; 
#endif
};

void write_bmp(char* filename, unsigned int width, unsigned int height, struct pixel* pixels);

#endif