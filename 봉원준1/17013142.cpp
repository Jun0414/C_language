#include <stdio.h>
#include <math.h>
#include<stdlib.h>
typedef unsigned char uint8;
typedef unsigned int uint32;
#define PI 3.14159265
#define SIZE 256

uint8** memory_alloc2D(uint32 height, uint32 width) {
	uint8**   ppMem2D = 0; 
	uint32    j, i;

	
	ppMem2D = (uint8**)calloc(sizeof(uint8*), height);
	if (ppMem2D == 0) {
		return 0;
	}

	*ppMem2D = (uint8*)calloc(sizeof(uint8), width * height);
	if ((*ppMem2D) == 0) {   
		free(ppMem2D);
		return 0;
	}

	for (j = 1; j < height; j++) {
		ppMem2D[j] = ppMem2D[j - 1] + width;
	}

	return ppMem2D;
}
int memory_free2D(uint8** ppMemAllocated) {
	if (ppMemAllocated == 0) {
		return -1;
	}

	free(ppMemAllocated[0]);
	free(ppMemAllocated);

	return 0;
}

int memory_free2D_int(int** ppMemAllocated) {
	if (ppMemAllocated == 0) {
		return -1;
	}

	free(ppMemAllocated[0]);
	free(ppMemAllocated);

	return 0;
}
int main(void) {
	FILE*     fpInputImage = 0;
	FILE*     fpOutputImage = 0;
	uint8**   ppInputImageBuffer = 0;
	uint8**   ppOutputImageBuffer = 0;
	int**   transInputImageBuffer = 0;
	int i, j, s, t;

	double array[3][3] = { { 1 ,1 ,1 },{ 1,1 ,1 },{ 1,1,1 } };
	
	fpInputImage = fopen("lena256x256.img", "rb");

	ppInputImageBuffer = (unsigned char**)malloc(sizeof(unsigned char*) * SIZE);
	for (i = 0; i < SIZE; i++) {
		ppInputImageBuffer[i] = (unsigned char*)malloc(sizeof(unsigned char) * SIZE);
	}
	ppOutputImageBuffer = (unsigned char**)malloc(sizeof(unsigned char*) * SIZE);
	for (i = 0; i < SIZE; i++) {
		ppOutputImageBuffer[i] = (unsigned char*)malloc(sizeof(unsigned char) * SIZE);
	}
	transInputImageBuffer = (int**)malloc(sizeof(int*) * SIZE);
	for (i = 0; i < SIZE; i++) {
		transInputImageBuffer[i] = (int*)malloc(sizeof(int) * SIZE);
	}

	for (i = 0; i < SIZE; i++) {
		fread(ppInputImageBuffer[i], sizeof(unsigned char), SIZE, fpInputImage);
	}



	for (i = 1; i < SIZE - 1; i++) {
		for (j = 1; j < SIZE - 1; j++) {
			transInputImageBuffer[i][j] = 0;
			for (s = -1; s <= 1; s++) {
				for (t = -1; t <= 1; t++) {
					transInputImageBuffer[i][j] += (array[s + 1][t + 1] * ppInputImageBuffer[i + s][j + t]) / 9.0;
				}
			}
		}
	}

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (transInputImageBuffer[i][j] > 225) transInputImageBuffer[i][j] = 225;
			else if (transInputImageBuffer[i][j] < 0) transInputImageBuffer[i][j] = 0;
		}
	}



	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			ppOutputImageBuffer[i][j] = (unsigned char)transInputImageBuffer[i][j];
		}
	}

	
	fpOutputImage = fopen("LPF.img", "wb");
	
	for (i = 0; i < SIZE; i++) {
		fwrite(ppOutputImageBuffer[i], sizeof(unsigned char), SIZE, fpOutputImage);
	}
	memory_free2D(ppInputImageBuffer);
	memory_free2D_int(transInputImageBuffer);
	fclose(fpInputImage);
	fclose(fpOutputImage);

	return 0;
}
