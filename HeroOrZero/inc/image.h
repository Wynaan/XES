#ifndef IMAGE_H_
#define IMAGE_H_
#include <stdint.h>
#include "interfaces.h"

class Image : public IRender{
private:
	const uint8_t * pixelArray;
	int16_t x, y;
	uint16_t width, height;
	uint8_t bgColor;
public:
	void Draw(rectangle_t drawBox);

	Image(const uint8_t * pImage, int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t bgColor) :
	pixelArray(pImage),
	x(x),
	y(y),
	width(width),
	height(height),
	bgColor(bgColor)
	{};
};



#endif /* IMAGE_H_ */
