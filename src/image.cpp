#include "image.h"
#include <stdint.h>

#define SSP_DATA (*((uint32_t *)0x40088008))

void Image::Draw(rectangle_t drawBox)
{
	uint16_t arraySize = width * height;

	for(uint16_t i = 0; i < arraySize; i++){
		if(this->pixelArray[i] != bgColor){
			uint32_t PixelData = 0x80000000 + (((((i / width) + y) * 0x420) + (i % width) + x) << 8) + pixelArray[i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
}


