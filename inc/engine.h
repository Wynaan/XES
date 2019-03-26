#ifndef ENGINE_H_
#define ENGINE_H_
#include "interfaces.h"
#include "map.h"
#include "xav_data.h"

enum class Stack{L1, L2, L3, L4, L5};

/* On each call to the Tick method, the TickEngine will call the Update method of
 * the first object contained on each stack level. This means elements meant to be
 * updated in parallel should be on different stack levels, L1 being the first one
 * to be updated. Putting an element on the same stack level as another element means
 * the previous element won't be updated until the new element is popped off the stack. */
class TickEngine{
public:
	static void PushStack(IUpdateable * element, Stack stackLevel);
	static void PopStack(Stack stackLevel);
	static void Tick();
};



#endif /* ENGINE_H_ */
