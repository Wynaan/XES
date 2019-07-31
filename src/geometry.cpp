#include "geometry.h"
#include "lpc_xav.h"
#include "math.h"

extern SSP DDC860_Bus;
extern bool Transfer_Done;
#define SSP_DATA (*((uint32_t *)0x40088008))

void Draw_Horizontal_Line(uint16_t x, uint16_t y, uint16_t end_x, uint8_t color)
{
	uint32_t Pixel_Address = 0x800000 + (y * 0x420) + x;
	uint16_t i = 0;

	while(1){
		SSP_DATA = ((Pixel_Address >> 8) & 0xFFFF);
		SSP_DATA = (((Pixel_Address & 0xFF) << 8) | color);
		for(uint8_t i = 0; i < 9; i++);
		Pixel_Address += 1;

		if((x + i) == end_x)
			break;

		i++;
	}
}

int16_t sign(int16_t x)
{
	if(x>0)
		return 1;
	else if(x<0)
		return -1;
	else
		return 0;
}

void Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color)
{
	uint32_t Pixel_Address;
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	int16_t temp;
    if (steep){
        temp = x0;
        x0 = y0;
        y0 = temp;

        temp = x1;
        x1 = y1;
        y1 = temp;
    }

    if (x0 > x1)
	{
        temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    }
    else {
        ystep = -1;
    }

    for (; x0<=x1; x0++)
    {
        if (steep)
            Pixel_Address = 0x80000000 + (((x0 * 0x420) + y0) << 8) + color;
        else
            Pixel_Address = 0x80000000 + (((y0 * 0x420) + x0) << 8) + color;

        SSP_DATA = (Pixel_Address >> 16) & 0xFFFF;
		SSP_DATA = Pixel_Address & 0xFFFF;
		for(uint8_t i = 0; i < 9; i++);

        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

void Draw_Circle(uint16_t x_center, uint16_t y_center, uint16_t radius, uint8_t color)
{
	uint16_t x = 0, y = radius;
	int16_t d = 3 - 2 * radius;

	Draw_Horizontal_Line(x_center - x, y_center + y, x_center + x, color);
	Draw_Horizontal_Line(x_center - x, y_center - y, x_center + x, color);
	Draw_Horizontal_Line(x_center - y, y_center + x, x_center + y, color);
	Draw_Horizontal_Line(x_center - y, y_center - x, x_center + y, color);

	while (y >= x)
	{
	    x++;

	    if (d > 0) { // decision parameter for under/over ideal circle line
	    	y--;
	        d = d + 4 * (x - y) + 10;
	    }
	    else{
	    	d = d + 4 * x + 6;
	    }

	    Draw_Horizontal_Line(x_center - x, y_center + y, x_center + x, color);
	    Draw_Horizontal_Line(x_center - x, y_center - y, x_center + x, color);
	    Draw_Horizontal_Line(x_center - y, y_center + x, x_center + y, color);
	    Draw_Horizontal_Line(x_center - y, y_center - x, x_center + y, color);
	}
}

void Draw_Plus(uint16_t xtop, uint16_t ytop, uint8_t size, uint8_t color)
{
	Draw_Line(xtop, ytop + (size >> 1), xtop + size, ytop + (size >> 1), color);
	Draw_Line(xtop + (size >> 1), ytop, xtop + (size >> 1), ytop + size, color);
}

void Draw_Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color)
{
	Draw_Horizontal_Line(x, y, x + width - 1, color);
	Draw_Horizontal_Line(x, y + height - 1, x + width - 1, color);
	Draw_Line(x, y, x, y + height - 1, color);
	Draw_Line(x + width - 1, y, x + width - 1, y + height - 1, color);
}

void Rectangle::Draw(rectangle_t drawBox){
	Draw_Rectangle(x, y, width, height, color);
}

Rectangle::Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color) :
	x(x),
	y(y),
	width(width),
	height(height),
	color(color)
{}
