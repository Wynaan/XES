#include "board.h"
#include "engine.h"
#include "xav_data.h"
#include "interfaces.h"
#include <list>

std::list<IUpdateable*> PriorityQueue[5];
Stack stackDepth = Stack::L1;

void TickEngine::PushStack(IUpdateable * element, Stack stackLevel)
{
	if(stackLevel > stackDepth)
		stackDepth = stackLevel;

	uint8_t accessor = (uint8_t)stackLevel;
	/* Insert the element at the beginning of the list */
	PriorityQueue[accessor].push_front(element);
}

void TickEngine::PopStack(Stack stackLevel)
{
	uint8_t accessor = (uint8_t)stackLevel;
	/* Delete the first element */
	PriorityQueue[accessor].pop_front();

	/* Resize the maximum stack depth if the only element was removed */
	if(PriorityQueue[accessor].size() == 0 && stackDepth == stackLevel)
		stackDepth = (Stack)(accessor - 1);
}

void TickEngine::Tick()
{
	/* Tick the first element for each priority level if defined */
	for(uint8_t i = 0; i <= (uint8_t)stackDepth; i++){
		if(PriorityQueue[i].size() != 0)
			(*PriorityQueue[i].begin())->Tick();
	}
}

