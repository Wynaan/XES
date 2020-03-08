#ifndef INTERFACES_H_
#define INTERFACES_H_
#include "xav_data.h"
#include "ps2controller.h"

/* IRender is the interface that all classes that intend to draw something on the screen
 * must inherit from. Each subclass must implement the Draw() method. This function must be
 * implement in such a way that the Renderer::AddToFrame method can push a pointer to the
 * object as well as the copy of the drawBox of this object, declared here, to two vectors.
 * The Renderer::Update method will iterate through these vectors to render the next frame.
 * Alternatively, IRender child classes can also be added to a special "callback" vector
 * using the Renderer::AddToCallback method, which will be rendered by the wrapper class
 * two frames after the function is called. */
class IRender{
public:
	rectangle_t drawBox;
	virtual void Draw(rectangle_t drawBox) = 0;
	virtual ~IRender() {}
};

/* IInteract is the interface that all classes that are intended for the user to actively
 * interact with, on the map, must inherit from. Each subclass must implement the OnUserInteract()
 * method. The Tile class can then create itself a new derived class object by using a pointer
 * to this base class and then perform the desired interaction. OnUserInteract will be called
 * by the Player entity using the input intended to perform an action. */
class IInteract{
protected:
	ps2_inputs_t userInput;
	ps2_buttons_t keyState;
public:
	virtual void OnUserInteract() = 0;
	virtual ~IInteract() {}
};

/* IUpdate is the interface that all classes defining mechanics and interactions that
 * must be updated on a frame by frame basis must inherit from. Each subclass must implement
 * the Tick() method. Afterward, entities and objects can add themselves, or other components,
 * to the update queue by calling the TickEngine::PushStack() method. The TickEngine wrapper
 * class implements a priority stack that will only update the first element on each priority
 * level, allowing for simple overriding of any component's behavior that must not occur
 * simultaneously. When a component is done with it's behavior, TickEngine::PopStack must be
 * called with the priority level to pop. */
class IUpdate{
public:
	virtual void Tick() = 0;
};



#endif /* INTERFACES_H_ */
