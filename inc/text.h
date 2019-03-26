#ifndef TEXT_H_
#define TEXT_H_
#include <stdint.h>
#include "interfaces.h"
#include <cstring>

void DrawStrConst(const char *StrToDraw, uint16_t x, uint16_t y, uint8_t fontWidth, uint8_t color);

class Text : public IRender{
private:
	const char *str;
	uint16_t x, y;
	uint8_t fontWidth, color;
public:
	void Draw(rectangle_t drawBox);

	Text(const char *StrToDraw, uint16_t x, uint16_t y, uint8_t fontWidth, uint8_t color) :
	str(StrToDraw),
	x(x),
	y(y),
	fontWidth(fontWidth),
	color(color)
	{}
};

#endif /* TEXT_H_ */
