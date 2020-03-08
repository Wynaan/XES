#ifndef GEOMETRY_H_
#define GEOMETRY_H_
#include <stdint.h>
#include "interfaces.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void Clear_Screen();
void Draw_Horizontal_Line(uint16_t x, uint16_t y, uint16_t end_x, uint8_t color);
void Draw_Line(uint16_t x, uint16_t y, uint16_t end_x, uint16_t end_y, uint8_t color);
void Draw_Circle(uint16_t x_center, uint16_t y_center, uint16_t radius, uint8_t color);
void Draw_Plus(uint16_t xtop, uint16_t ytop, uint8_t size, uint8_t color);
void Draw_Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color);

class Rectangle : public IRender{
	uint16_t x, y;
	uint16_t width, height;
	uint8_t color;
public:
	void Draw(rectangle_t drawBox);

	Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color);
};

#endif /* GEOMETRY_H_ */
