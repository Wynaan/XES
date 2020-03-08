#ifndef BENCHMARKLOGGER_H_
#define BENCHMARKLOGGER_H_
#include "board.h"

static uint32_t tStart;
static uint32_t tEnd;
static uint32_t result = {0};

class Benchmark{
public:
	static void Start(){tStart = LPC_TIMER0->TC;}
	static void End(){tEnd = LPC_TIMER0->TC;}
	static void Translate_us(){result = (tEnd - tStart) / 96;}
};


#endif /* BENCHMARKLOGGER_H_ */
