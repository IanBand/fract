// based on https://gist.github.com/binshengliu/1723715
#include "bmp.h"


void write_bmp(char* filename, unsigned int width, unsigned int height, struct pixel* pixels) {
	FILE *file = fopen(filename, "w+b");  
	static unsigned char header[54] = {66,77,0,0,0,0,0,0,0,0,54,0,0,0,40,0,0,0,0,0,0,0,0,0,0,0,1,0,24}; //rest is zeroes
	unsigned int pixelBytesPerRow = width * sizeof(struct pixel);
	unsigned int paddingBytesPerRow = (4-(pixelBytesPerRow%4))%4;
	unsigned int* sizeOfFileEntry = (unsigned int*) &header[2];
	*sizeOfFileEntry = 54 + (pixelBytesPerRow + paddingBytesPerRow) * height;  
	unsigned int* widthEntry = (unsigned int*) &header[18];    
	*widthEntry = width;
	unsigned int* heightEntry = (unsigned int*) &header[22];    
	*heightEntry = height;

	fwrite(&header, 54, 1, file);

	static unsigned char zeroes[3] = {0,0,0}; //for padding    
	for (int row = 0; row < height; row++) {
		fwrite(&pixels[ row * width ], pixelBytesPerRow, 1, file);
		fwrite(&zeroes, paddingBytesPerRow, 1, file);
	}
	fclose(file);
}