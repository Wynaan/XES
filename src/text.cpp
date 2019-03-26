#include "text.h"
#include "font.h"
#include "xav_data.h"
#include <stdint.h>

#define SSP_DATA (*((uint32_t *)0x40088008))

void DrawStrConst(const char *StrToDraw, uint16_t x, uint16_t y, uint8_t fontWidth, uint8_t color)
{
	uint16_t Offset = 0;
	uint16_t arraySize = (fontWidth == 16)? 304 : 192;
	const uint16_t * _font;
	bool valid = true;

	for(int i = 0; StrToDraw[i] != '\0'; i++){
		valid = true;
		if(i != 0){
			switch(StrToDraw[i - 1]){
				case 'i':
				case 'j':
				case 'l':
				case 'I':
					Offset += (fontWidth == 16)? 4 : 3;
					break;
				case 'r':
				case 'f':
				case '1':
				case 't':
				case ' ':
					Offset += (fontWidth == 16)? 8 : 5;
					break;
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'g':
				case 'h':
				case 'k':
				case 'n':
				case 'o':
				case 'p':
				case 'q':
				case 's':
				case 'u':
				case 'v':
				case 'x':
				case 'y':
				case 'z':
				case '0':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case 'L':
				case 'T':
				case 'F':
					Offset += (fontWidth == 16)? 12 : 7;
					break;
				case 'W':
					Offset += (fontWidth == 16)? 16: 12;
					break;
				default:
					Offset += fontWidth;
					break;
			}
		}
		if(StrToDraw[i] >= 'a' && StrToDraw[i] <= 'z')
			_font = (fontWidth == 16)? OTF_16X19[StrToDraw[i] - 'a'] : OTF_10X12[StrToDraw[i] - 'a'];
		else if(StrToDraw[i] >= 'A' && StrToDraw[i] <= 'Z')
			_font = (fontWidth == 16)? OTF_16X19[StrToDraw[i] - 'A' + 26] : OTF_10X12[StrToDraw[i] - 'A' + 26];
		else if(StrToDraw[i] >= '0' && StrToDraw[i] <= '9')
			 _font = (fontWidth == 16)? OTF_16X19[StrToDraw[i] - '0' + 52] : OTF_10X12[StrToDraw[i] - '0' + 52];
		else{
			switch(StrToDraw[i]){
				case '/':
					_font = OTF_10X12[62];
					break;
				case ':':
					_font = OTF_10X12[63];
					break;
				case '+':
					_font = OTF_10X12[64];
					break;
				default:
					valid = false;
					break;
			}
		}

		if(valid){
			for(int j = 0; j < arraySize; j++){
				if(_font[j / 16] & (1 << (j % 16))){
					uint32_t PixelData = 0x80000000 + ((((y + (j / 16)) * 0x420) + x + (j % 16) + Offset) << 8) + color;
					SSP_DATA = ((PixelData >> 16) & 0xFFFF);
					SSP_DATA = (PixelData & 0xFFFF);
					for(uint8_t i = 0; i < 9; i++);
				}
			}
		}
	}
}

void Text::Draw(rectangle_t drawBox)
{
	DrawStrConst(str, x, y, fontWidth, color);
}


