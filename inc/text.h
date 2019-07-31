#ifndef TEXT_H_
#define TEXT_H_
#include <stdint.h>
#include "interfaces.h"
#include <cstring>

void DrawStrConst(const char *StrToDraw, uint16_t x, uint16_t y, uint8_t fontWidth, uint8_t color);

class Text : public IRender{
private:
	typedef enum {TEXT, DIGITS}char_type_e;
	char_type_e type;
	const char *str;
	uint32_t nb;
	uint16_t x, y;
	uint8_t fontWidth, color;
public:
	void Draw(rectangle_t drawBox);

	Text(const char *StrToDraw, uint16_t x, uint16_t y, uint8_t fontWidth, uint8_t color);
	Text(const uint32_t NbToDraw, uint16_t x, uint16_t y, uint8_t fontWidth, uint8_t color);
};

#endif /* TEXT_H_ */
