#ifndef SEGDISPLAY_H_
#define SEGDISPLAY_H_
#include <stdint.h>

static unsigned long volatile * const PtrRI_TIMER_COMPVAL = (unsigned long *) 0x400B0000;
static unsigned long volatile * const PtrRI_TIMER_MASK = (unsigned long *) 0x400B0004;
static unsigned long volatile * const PtrRI_TIMER_CTRL = (unsigned long *) 0x400B0008;
static unsigned long volatile * const PtrRI_TIMER_COUNT = (unsigned long *) 0x400B000C;
#define RI_TIMER_COMPVAL *PtrRI_TIMER_COMPVAL
#define RI_TIMER_MASK *PtrRI_TIMER_MASK
#define RI_TIMER_CTRL *PtrRI_TIMER_CTRL
#define RI_TIMER_COUNT *PtrRI_TIMER_COUNT

typedef union {
    uint32_t     DDabble_Container;

    struct {
        uint16_t uint16_dec; // Source 16-bit uint will be assigned here
        unsigned uni    :4;  // Converted individual BCD numbers
        unsigned diz    :4;  // Will be contained here
        unsigned cen    :4;
        unsigned mil    :4;
    };
} BCD_t;

extern BCD_t Seg_BCD;
extern BCD_t Console_BCD;
extern BCD_t LCD_BCD;

void IntegerToBCD(BCD_t& BCD_Container, uint16_t Decimal_uint); // Running this function modifies the content of BCD struct

void PrintDigit(char AsciiNumber);

void WriteStrConst(const char *StrToSend);

void PrintDec(unsigned int NbrDec);

void RIT_Init();

#endif /* SEGDISPLAY_H_ */
