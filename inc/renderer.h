#ifndef RENDERER_H_
#define RENDERER_H_
#include <cr_section_macros.h>
#include "xav_data.h"
#include "interfaces.h"
#include <stdint.h>

void ClearScreen();
void MemorySwap();

class Renderer{
private:
	/* Disallows instanciation of Renderer class */
	Renderer();
public:
	/* Adds the object for rendering on the next frame */
	static void AddToFrame(IRender * object);
	/* Adds the object for rendering on the next two frames */
	static void AddToDualFrame(IRender * object);
	/* Adds the object for rendering in the 3rd next frame */
	static void AddToCallback(IRender * object);
	/* Renders all the objects in the IRender lists and swaps frame buffers */
	static void Update();
	/* Renders all the objects in the IRender lists once in each frame buffer */
	static void Init();
};



#endif /* RENDERER_H_ */
