#include <segdisplay.h>
#include <stdint.h>
#include "lpc_xav.h"

extern Bus DisplayDataBus;
extern Bus DisplaySelectBus;

extern BCD_t Seg_BCD;

void IntegerToBCD(BCD_t& BCD_Container, uint16_t Decimal_uint)
{
    BCD_Container.DDabble_Container = (0x0000FFFF & Decimal_uint); // Clear the BCD struct

    // Converts 16-bit int to BCD in separate variables
    for(char i = 0; i < 16; i++){
        if(BCD_Container.uni > 4)
            BCD_Container.uni += 3;
        if(BCD_Container.diz > 4)
            BCD_Container.diz += 3;
        if(BCD_Container.cen > 4)
            BCD_Container.cen += 3;
        if(BCD_Container.mil > 4)
            BCD_Container.mil += 3;
        BCD_Container.DDabble_Container <<= 1;
    }
}

void PrintDigit(char AsciiNumber)
{
	if(AsciiNumber <= 9)
		DisplayDataBus = AsciiNumber;
	else if(AsciiNumber <= 57)
		DisplayDataBus = AsciiNumber - 48;
	else if(AsciiNumber >= 65)
		DisplayDataBus = AsciiNumber - 55;
	else
		switch(AsciiNumber)
		{
		case '=':
			DisplayDataBus = 43;
			break;
		case ' ':
			DisplayDataBus = 48;
			break;
		}
}

void WriteStrConst(const char *StrToSend) // Obsolete with RIT seg-by-seg mode
{
    for(; *StrToSend; StrToSend++){
        PrintDigit(*StrToSend);
        for(uint32_t i = 0; i< 5000; i++);
        PrintDigit(' '); // Sends a dead segment (space) to prevent shadows of precedent digit
        DisplaySelectBus += 1;
    }
}

void PrintDec(unsigned int NbrDec) // Obsolete with RIT seg-by-seg mode
{
    IntegerToBCD(Seg_BCD, NbrDec);

    DisplaySelectBus = 0;

    DisplayDataBus = (Seg_BCD.mil);
    for(uint32_t i = 0; i< 5000; i++);
    DisplaySelectBus += 1;

    DisplayDataBus = (Seg_BCD.cen);
    for(uint32_t i = 0; i< 5000; i++);
    DisplaySelectBus += 1;

    DisplayDataBus = (Seg_BCD.diz);
    for(uint32_t i = 0; i< 5000; i++);
    DisplaySelectBus += 1;

    DisplayDataBus = (Seg_BCD.uni);
    for(uint32_t i = 0; i< 5000; i++);
}

void RIT_Init() // Initialize RIT peripheral
{
	*(uint32_t *)0x400FC0C4 |= (1 << 16);
	RI_TIMER_COMPVAL = 50000;
	RI_TIMER_COUNT = 0;
	RI_TIMER_CTRL = 0x0E;
	EnableIRQ(29);
}



