#include "renderer.h"
#include "lpc_xav.h"
#include <utility>
#include <deque>

#define SSP_DATA (*((uint32_t *)0x40088008))

extern LPC_IO VSYNC, CS;

std::deque<std::pair<IRender*, rectangle_t> > RenderList;
std::deque<std::pair<IRender*, rectangle_t> > DoubleRenderList[3];
std::deque<std::pair<IRender*, rectangle_t> > CallbackList[4];
uint8_t cListIndex = 0;
uint8_t dListIndex = 0;

void ClearScreen()
{
	for(uint8_t i = 0; i < 2; i++){
		uint32_t Pixel_Address = 0x800000;
		MemorySwap();
		while(1){
			SSP_DATA = ((Pixel_Address >> 8) & 0xFFFF);
			SSP_DATA = ((Pixel_Address & 0xFF) << 8);
			for(uint8_t j = 0; j < 9; j++);
			Pixel_Address += 1;
			if(Pixel_Address > 0x89FF00)
				break;
		}
	}
}

void MemorySwap()
{
	static bool Mem_Select = 0;
	Mem_Select = !Mem_Select;
	CS = Mem_Select;
	while(VSYNC != 1);
	while(VSYNC != 0);
}

void Renderer::AddToFrame(IRender * object)
{
	RenderList.push_back(std::make_pair(object, object->drawBox));
}

void Renderer::AddToDualFrame(IRender * object)
{
	DoubleRenderList[(dListIndex + 1) % 3].push_back(std::make_pair(object, object->drawBox));
}

void Renderer::AddToCallback(IRender * object)
{
	/* Place all objects defined as erasable in a vector
	 * so they are in the 3rd next frame */
	CallbackList[cListIndex].push_back(std::make_pair(object, object->drawBox));
}

void Renderer::Update()
{
	/* Draw objects added to callback list two updates ago */
	for(auto&& i : CallbackList[(cListIndex + 2) % 4])
		i.first->Draw(i.second);
	/* Draw objects in the double render list */
	for(auto&& i : DoubleRenderList[dListIndex % 3])
		i.first->Draw(i.second);
	for(auto&& i : DoubleRenderList[(dListIndex + 1) % 3])
		i.first->Draw(i.second);
	/* Draw objects in the render queue */
	for(auto&& i : RenderList)
		i.first->Draw(i.second);

	/* Cleanup queues */
	RenderList.clear();
	DoubleRenderList[(dListIndex + 2) % 3].clear();
	CallbackList[(cListIndex + 2) % 4].clear();

	/* Increment index */
	cListIndex++;
	cListIndex %= 4;
	dListIndex++;
	dListIndex %= 3;

	/* Toggle CS pin and wait for VSync to go high */
	MemorySwap();
}

void Renderer::Init()
{
	std::deque<std::pair<IRender*, rectangle_t> >::iterator it;

	for(int i = 0; i < 2; i++){
		/* Draw every object in the render queue */
		for(auto i : RenderList)std::get<IRender*>(i)->Draw(std::get<rectangle_t>(i));
		/* Toggle CS pin and wait for VSync to go high */
		MemorySwap();
	}
	/* Cleanup vectors */
	RenderList.clear();
}



