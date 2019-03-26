#include "board.h"
#include "gui.h"
#include "user_interface.h"
#include "xav_data.h"
#include <cstdlib>
#include "text.h"
#include "geometry.h"
#include "renderer.h"
#include "map.h"
#include <deque>
#include "game.h"

#define ROUGE 		0xE0
#define ORANGE 		0xF0
#define JAUNE		0xFC
#define VERT		0x1C
#define TURQUOISE 	0x1F
#define BLEU		0x03
#define VIOLET		0x83
#define ROSE		0xE3
#define BLANC		0xFF
#define NOIR		0x00

void DrawMenuArrow(uint16_t x, uint16_t y, uint8_t color, bool mirrored)
{
	if(!mirrored){
		/* Horizontal segment */
		Draw_Line(x + 1, y - 1, x + 19, y - 1, color);
		Draw_Line(x, y, x + 20, y, color);
		Draw_Line(x + 1, y + 1, x + 19, y + 1, color);
		/* Top segment */
		Draw_Line(x + 11, y - 9, x + 19, y - 1, color);
		Draw_Line(x + 11, y - 8, x + 19, y, color);
		Draw_Line(x + 11, y - 7, x + 19, y + 1, color);
		/* Bottom segment */
		Draw_Line(x + 11, y + 7, x + 19, y - 1, color);
		Draw_Line(x + 11, y + 8, x + 19, y, color);
		Draw_Line(x + 11, y + 9, x + 19, y + 1, color);
	}
	else{
		/* Horizontal segment */
		Draw_Line(x + 1, y - 1, x + 19, y - 1, color);
		Draw_Line(x, y, x + 20, y, color);
		Draw_Line(x + 1, y + 1, x + 19, y + 1, color);
		/* Top segment */
		Draw_Line(x + 1, y - 1, x + 9, y - 9, color);
		Draw_Line(x + 1, y, x + 9, y - 8, color);
		Draw_Line(x + 1, y + 1, x + 9, y - 7, color);
		/* Bottom segment */
		Draw_Line(x + 1, y - 1, x + 9, y + 7, color);
		Draw_Line(x + 1, y, x + 9, y + 8, color);
		Draw_Line(x + 1, y + 1, x + 9, y + 9, color);
	}
}

void Background::Draw(rectangle_t drawBox)
{
	/* Draw top row */
	for(uint16_t i = 0; i < 400; i++){
		if(bgTable[0][i] != 0xFF){
			uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y) * 0x420) + (i % 20) + pos.x) << 8) + bgTable[0][i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
	for(uint8_t j = 0; j < sWidth; j++){
		for(uint16_t i = 0; i < 400; i++){
			if(bgTable[3][i] != 0xFF){
				uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y) * 0x420) + (i % 20) + pos.x + (j * 20) + 20) << 8) + bgTable[3][i];
				SSP_DATA = ((PixelData >> 16) & 0xFFFF);
				SSP_DATA = (PixelData & 0xFFFF);
				for(uint8_t i = 0; i < 9; i++);
			}
		}
	}
	for(uint16_t i = 0; i < 400; i++){
		if(bgTable[6][i] != 0xFF){
			uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y) * 0x420) + (i % 20) + pos.x + (sWidth * 20) + 20) << 8) + bgTable[6][i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
	/* Draw middle section */
	for(uint8_t k = 0; k < sHeight; k++){
		for(uint16_t i = 0; i < 400; i++){
			if(bgTable[1][i] != 0xFF){
				uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y + (k * 20) + 20) * 0x420) + (i % 20) + pos.x) << 8) + bgTable[1][i];
				SSP_DATA = ((PixelData >> 16) & 0xFFFF);
				SSP_DATA = (PixelData & 0xFFFF);
				for(uint8_t i = 0; i < 9; i++);
			}
		}
		for(uint8_t j = 0; j < sWidth; j++){
			for(uint16_t i = 0; i < 400; i++){
				if(bgTable[4][i] != 0xFF){
					uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y + (k * 20) + 20) * 0x420) + (i % 20) + pos.x + (j * 20) + 20) << 8) + bgTable[4][i];
					SSP_DATA = ((PixelData >> 16) & 0xFFFF);
					SSP_DATA = (PixelData & 0xFFFF);
					for(uint8_t i = 0; i < 9; i++);
				}
			}
		}
		for(uint16_t i = 0; i < 400; i++){
			if(bgTable[7][i] != 0xFF){
				uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y + (k * 20) + 20) * 0x420) + (i % 20) + pos.x + (sWidth * 20) + 20) << 8) + bgTable[7][i];
				SSP_DATA = ((PixelData >> 16) & 0xFFFF);
				SSP_DATA = (PixelData & 0xFFFF);
				for(uint8_t i = 0; i < 9; i++);
			}
		}
	}
	/* Draw bottom row */
	for(uint16_t i = 0; i < 400; i++){
		if(bgTable[2][i] != 0xFF){
			uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y + (sHeight * 20) + 20) * 0x420) + (i % 20) + pos.x) << 8) + bgTable[2][i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
	for(uint8_t j = 0; j < sWidth; j++){
		for(uint16_t i = 0; i < 400; i++){
			if(bgTable[5][i] != 0xFF){
				uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y + (sHeight * 20) + 20) * 0x420) + (i % 20) + pos.x + (j * 20) + 20) << 8) + bgTable[5][i];
				SSP_DATA = ((PixelData >> 16) & 0xFFFF);
				SSP_DATA = (PixelData & 0xFFFF);
				for(uint8_t i = 0; i < 9; i++);
			}
		}
	}
	for(uint16_t i = 0; i < 400; i++){
		if(bgTable[8][i] != 0xFF){
			uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y + (sHeight * 20) + 20) * 0x420) + (i % 20) + pos.x + (sWidth * 20) + 20) << 8) + bgTable[8][i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
}

Background::Background(coords_pair_t position, uint8_t sectionWidth, uint8_t sectionHeight)
{
	pos = position;
	sWidth = sectionWidth;
	sHeight = sectionHeight;
	tWidth = 20;
	tHeight = 20;

	drawBox.topleft.x = pos.x;
	drawBox.topleft.y = pos.y;
	drawBox.botright.x = pos.x + sWidth * 20 + 40;
	drawBox.botright.y = pos.y + sHeight * 20 + 40;
}

void Button::Draw(rectangle_t drawBox)
{
	for(uint16_t i = 0; i < 800; i++){
		if(pImage[0][i] != 0x00){
			uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y) * 0x420) + (i % 20) + pos.x) << 8) + pImage[0][i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
	for(uint8_t j = 0; j < sWidth; j++){
		for(uint16_t i = 0; i < 800; i++){
			if(pImage[1][i] != 0x00){
				uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y) * 0x420) + (i % 20) + pos.x + (j * 20) + 20) << 8) + pImage[1][i];
				SSP_DATA = ((PixelData >> 16) & 0xFFFF);
				SSP_DATA = (PixelData & 0xFFFF);
				for(uint8_t i = 0; i < 9; i++);
			}
		}
	}
	for(uint16_t i = 0; i < 800; i++){
		if(pImage[2][i] != 0x00){
			uint32_t PixelData = 0x80000000 + (((((i / 20) + pos.y) * 0x420) + (i % 20) + pos.x + (sWidth * 20) + 20) << 8) + pImage[2][i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
}

Button::Button(btn_color_e color, coords_pair_t position, uint8_t sectionWidth)
{
	pos = position;
	sWidth = sectionWidth;
	sHeight = 0;
	tWidth = 20;
	tHeight = 40;

	if(color == Button::GREEN){
		pImage[0] = greenButton[0];
		pImage[1] = greenButton[1];
		pImage[2] = greenButton[2];
	}
	else{
		pImage[0] = redButton[0];
		pImage[1] = redButton[1];
		pImage[2] = redButton[2];
	}
}

void UserInterface::AddElement(IRender* item)
{
	this->items.push_back(item);
}

bool UserInterface::ProcessGraphics(bool exit)
{
	switch(state){
	case 0: // Initial entry
		Renderer::AddToDualFrame(bg);
		for(auto i : items) Renderer::AddToDualFrame(i);
	case 1:
		state++;
		break;
	case 2:
		/* delete the GUI elements  */
		for(auto i : items) delete i;
		items.clear();
		state++;
	case 3:
		for(auto i : items) Renderer::AddToDualFrame(i);
		if(exit) state = 9;
		break;
	case 9: // Case 9 handles erasal of the UI
		for(uint8_t y = 3; y < 13; y++){
			for(uint8_t x = 4; x < 16; x++){
				if(WorldManager::worldArray[x][y]->CheckTileOverlap(bg->drawBox))
					Renderer::AddToDualFrame(WorldManager::worldArray[x][y]);
			}
		}
		state++;
		break;
	case 10: // Case 11 handles returns false to signify completion
		state = 0;
		return false;
		break;
	}

	return true;
}

UserInterface::UserInterface(int16_t x, int16_t y, uint8_t sectionWidth, uint8_t sectionHeight) :
sWidth(sectionWidth),
sHeight(sectionHeight)
{
	pos.x = x;
	pos.y = y;
	state = 0;
	bg = new Background(this->pos, this->sWidth, this->sHeight);
}

void InfoDisplay::Draw(rectangle_t drawBox)
{
	/* Draw food icon */
	for(uint16_t i = 0; i < 728; i++){
		if(corn[i] != 0x00){
			uint32_t PixelData = 0x80000000 + (((((i / 28) + pos.y + 7) * 0x420) + (i % 28) + pos.x + 20) << 8) + corn[i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
	/* Draw wood icon */
	for(uint16_t i = 0; i < 858; i++){
		if(wood[i] != 0xFF){
			uint32_t PixelData = 0x80000000 + (((((i / 33) + pos.y + 7) * 0x420) + (i % 33) + pos.x + 20 + 140) << 8) + wood[i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
	/* Draw stone icon */
	for(uint16_t i = 0; i < 910; i++){
		if(stone[i] != 0xFF){
			uint32_t PixelData = 0x80000000 + (((((i / 35) + pos.y + 7) * 0x420) + (i % 35) + pos.x + 20 + 280) << 8) + stone[i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
	/* Draw iron icon */
	for(uint16_t i = 0; i < 780; i++){
		if(iron[i] != 0xFF){
			uint32_t PixelData = 0x80000000 + (((((i / 30) + pos.y + 7) * 0x420) + (i % 30) + pos.x + 20 + 420) << 8) + iron[i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
	/* Draw gold icon */
	for(uint16_t i = 0; i < 780; i++){
		if(gold[i] != 0xFF){
			uint32_t PixelData = 0x80000000 + (((((i / 30) + pos.y + 7) * 0x420) + (i % 30) + pos.x + 20 + 560) << 8) + gold[i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
}

void InfoDisplay::Load()
{
	Renderer::AddToFrame(this->bg);
	Renderer::AddToFrame(this);
}

void InfoDisplay::Tick()
{
	const uint8_t DONE = 6;
	static int8_t state = -2;
	static uint32_t oldTime = 0;
	static resources_t oldRes, oldProd;
	static resources_t currentRes = Game::Resources, currentProd = Game::ProdPer5;
	char temp[7];

	/* Updates the displayed values every 5 seconds */
	if((LPC_TIMER0->TC - oldTime) > 480000000 && (state == DONE || state == -2)){
		state = 0;
		oldTime = LPC_TIMER0->TC;
		Game::Resources += Game::ProdPer5;
		currentRes = Game::Resources;
		currentProd = Game::ProdPer5;
	}

	switch(state){
	case -2:
	case -1:
		Draw_Plus(pos.x + 60, pos.y + 23, 6, VERT);
		Draw_Plus(pos.x + 205, pos.y + 23, 6, VERT);
		Draw_Plus(pos.x + 345, pos.y + 23, 6, VERT);
		Draw_Plus(pos.x + 485, pos.y + 23, 6, VERT);
		Draw_Plus(pos.x + 625, pos.y + 23, 6, VERT);
		state++;
		break;
	case 0:
	case 1:
		itoa(oldRes.food, temp, 10);
		DrawStrConst(temp, pos.x + 60, pos.y + 9, 10, 0x24);

		itoa(oldRes.wood, temp, 10);
		DrawStrConst(temp, pos.x + 205, pos.y + 9, 10, 0x24);

		itoa(oldRes.stone, temp, 10);
		DrawStrConst(temp, pos.x + 345, pos.y + 9, 10, 0x24);

		itoa(oldRes.iron, temp, 10);
		DrawStrConst(temp, pos.x + 485, pos.y + 9, 10, 0x24);

		itoa(oldRes.gold, temp, 10);
		DrawStrConst(temp, pos.x + 625, pos.y + 9, 10, 0x24);

		itoa(currentRes.food, temp, 10);
		DrawStrConst(temp, pos.x + 60, pos.y + 9, 10, BLANC);

		itoa(currentRes.wood, temp, 10);
		DrawStrConst(temp, pos.x + 205, pos.y + 9, 10, BLANC);

		itoa(currentRes.stone, temp, 10);
		DrawStrConst(temp, pos.x + 345, pos.y + 9, 10, BLANC);

		itoa(currentRes.iron, temp, 10);
		DrawStrConst(temp, pos.x + 485, pos.y + 9, 10, BLANC);

		itoa(currentRes.gold, temp, 10);
		DrawStrConst(temp, pos.x + 625, pos.y + 9, 10, BLANC);

		state++;
		break;
	case 2:
		oldRes = currentRes;
		if(oldProd != currentProd)
			state++;
		else
			state = DONE;
		break;
	case 3:
	case 4:
		itoa(oldProd.food, temp, 10);
		DrawStrConst(temp, pos.x + 68, pos.y + 22, 10, 0x24);

		itoa(oldProd.wood, temp, 10);
		DrawStrConst(temp, pos.x + 213, pos.y + 22, 10, 0x24);

		itoa(oldProd.stone, temp, 10);
		DrawStrConst(temp, pos.x + 353, pos.y + 22, 10, 0x24);

		itoa(oldProd.iron, temp, 10);
		DrawStrConst(temp, pos.x + 493, pos.y + 22, 10, 0x24);

		itoa(oldProd.gold, temp, 10);
		DrawStrConst(temp, pos.x + 633, pos.y + 22, 10, 0x24);

		itoa(currentProd.food, temp, 10);
		DrawStrConst(temp, pos.x + 68, pos.y + 22, 10, VERT);

		itoa(currentProd.wood, temp, 10);
		DrawStrConst(temp, pos.x + 213, pos.y + 22, 10, VERT);

		itoa(currentProd.stone, temp, 10);
		DrawStrConst(temp, pos.x + 353, pos.y + 22, 10, VERT);

		itoa(currentProd.iron, temp, 10);
		DrawStrConst(temp, pos.x + 493, pos.y + 22, 10, VERT);

		itoa(currentProd.gold, temp, 10);
		DrawStrConst(temp, pos.x + 633, pos.y + 22, 10, VERT);

		state++;
		break;
	case 5:
		oldProd = currentProd;
		state = DONE;
	default:
		break;
	}
}

InfoDisplay::InfoDisplay(uint8_t x, uint8_t y)
{
	this->pos.x = x;
	this->pos.y = y;

	bg = new Background(this->pos, 35, 0);
}
/*****************************************************************************************/
void TileSelector::Draw(rectangle_t drawBox)
{
	coords_pair_t tilePos = selectedTile->getPos();

	// northwest line
	Draw_Line(tilePos.x + 3, tilePos.y + 10, tilePos.x + 17, tilePos.y + 1, 0xFF);
	Draw_Line(tilePos.x + 3, tilePos.y + 11, tilePos.x + 17, tilePos.y + 2, 0xFF);
	// north corner
	Draw_Line(tilePos.x + 17, tilePos.y + 1, tilePos.x + 22, tilePos.y + 1, 0xFF);
	Draw_Line(tilePos.x + 17, tilePos.y + 2, tilePos.x + 22, tilePos.y + 2, 0xFF);
	// west line
	Draw_Line(tilePos.x, tilePos.y + 13, tilePos.x, tilePos.y + 32, 0xFF);
	Draw_Line(tilePos.x + 1, tilePos.y + 13, tilePos.x + 1, tilePos.y + 32, 0xFF);
	// northwest corner
	Draw_Line(tilePos.x, tilePos.y + 13, tilePos.x + 3, tilePos.y + 10, 0xFF);
	Draw_Line(tilePos.x + 1, tilePos.y + 13, tilePos.x + 3, tilePos.y + 11, 0xFF);
	// northeast line
	Draw_Line(tilePos.x + 22, tilePos.y + 1, tilePos.x + 36, tilePos.y + 10, 0xFF);
	Draw_Line(tilePos.x + 22, tilePos.y + 2, tilePos.x + 36, tilePos.y + 11, 0xFF);
	// northeast corner
	Draw_Line(tilePos.x + 36, tilePos.y + 10, tilePos.x + 38, tilePos.y + 13, 0xFF);
	Draw_Line(tilePos.x + 36, tilePos.y + 11, tilePos.x + 39, tilePos.y + 13, 0xFF);
	// southwest line
	Draw_Line(tilePos.x + 3, tilePos.y + 34, tilePos.x + 17, tilePos.y + 43, 0xFF);
	Draw_Line(tilePos.x + 3, tilePos.y + 35, tilePos.x + 17, tilePos.y + 44, 0xFF);
	// southwest corner
	Draw_Line(tilePos.x, tilePos.y + 32, tilePos.x + 3, tilePos.y + 34, 0xFF);
	Draw_Line(tilePos.x + 1, tilePos.y + 32, tilePos.x + 3, tilePos.y + 35, 0xFF);
	// southeast line
	Draw_Line(tilePos.x + 22, tilePos.y + 43, tilePos.x + 36, tilePos.y + 34, 0xFF);
	Draw_Line(tilePos.x + 22, tilePos.y + 44, tilePos.x + 36, tilePos.y + 35, 0xFF);
	// southeast corner
	Draw_Line(tilePos.x + 36, tilePos.y + 34, tilePos.x + 38, tilePos.y + 32, 0xFF);
	Draw_Line(tilePos.x + 36, tilePos.y + 35, tilePos.x + 39, tilePos.y + 32, 0xFF);
	// east line
	Draw_Line(tilePos.x + 38, tilePos.y + 13, tilePos.x + 38, tilePos.y + 32, 0xFF);
	Draw_Line(tilePos.x + 39, tilePos.y + 13, tilePos.x + 39, tilePos.y + 32, 0xFF);
	// south corner
	Draw_Line(tilePos.x + 17, tilePos.y + 43, tilePos.x + 22, tilePos.y + 43, 0xFF);
	Draw_Line(tilePos.x + 17, tilePos.y + 44, tilePos.x + 22, tilePos.y + 44, 0xFF);
}

void TileSelector::ProcessGraphics(bool showSelector)
{
	static Tile * lastPos = selectedTile;
	static bool lastSetting;
	coords_pair_t position;

	if(this->selectedTile != nullptr){
		switch(state){
		case 0:
			if(showSelector){
				if(!doneMoving)
					state = 1;
				else if(lastSetting != showSelector){
					state = 3;
					lastSetting = showSelector;
				}
			}
			else{
				if(lastSetting != showSelector){
					state = 5;
					lastSetting = showSelector;
				}
			}
			break;
		case 1: // Case where selector has moved
			position = lastPos->getPos();

			/* Place the drawbox at the previous tile the selector was */
			drawBox.topleft.x = position.x;
			drawBox.topleft.y = position.y;
			drawBox.botright.x = position.x + 39;
			drawBox.botright.y = position.y + 45;

			lastPos->CheckTileOverlap(drawBox);
			Renderer::AddToFrame(lastPos);

			lastPos->neighbor[SOUTH_WEST]->CheckTileOverlap(drawBox);
			Renderer::AddToFrame(lastPos->neighbor[SOUTH_WEST]);

			lastPos->neighbor[SOUTH_EAST]->CheckTileOverlap(drawBox);
			Renderer::AddToFrame(lastPos->neighbor[SOUTH_EAST]);

			Renderer::AddToFrame(this);

			state++;
			break;
		case 2: // Second pass through after selector move
			lastPos->CheckTileOverlap(this->drawBox);
			Renderer::AddToFrame(lastPos);

			lastPos->neighbor[SOUTH_WEST]->CheckTileOverlap(drawBox);
			Renderer::AddToFrame(lastPos->neighbor[SOUTH_WEST]);

			lastPos->neighbor[SOUTH_EAST]->CheckTileOverlap(drawBox);
			Renderer::AddToFrame(lastPos->neighbor[SOUTH_EAST]);

			Renderer::AddToFrame(this);

			doneMoving = true;
			lastPos = selectedTile;
			state = 0;
			break;
		case 3: // Case where showSelector is toggled on
			Renderer::AddToFrame(this);
			state++;
			break;
		case 4: // Second pass through after toggle on
			Renderer::AddToFrame(this);
			lastPos = selectedTile;
			state = 0;
			break;
		case 5: // Case where showSelector is toggled off
			position = selectedTile->getPos();

			/* Place the drawbox at the previous tile the selector was */
			drawBox.topleft.x = position.x;
			drawBox.topleft.y = position.y;
			drawBox.botright.x = position.x + 39;
			drawBox.botright.y = position.y + 45;

			selectedTile->CheckTileOverlap(this->drawBox);
			Renderer::AddToFrame(selectedTile);

			selectedTile->neighbor[SOUTH_WEST]->CheckTileOverlap(drawBox);
			Renderer::AddToFrame(selectedTile->neighbor[SOUTH_WEST]);

			selectedTile->neighbor[SOUTH_EAST]->CheckTileOverlap(drawBox);
			Renderer::AddToFrame(selectedTile->neighbor[SOUTH_EAST]);

			state++;
			break;
		case 6: // Second pass through after toggle off
			selectedTile->CheckTileOverlap(this->drawBox);
			Renderer::AddToFrame(selectedTile);

			selectedTile->neighbor[SOUTH_WEST]->CheckTileOverlap(drawBox);
			Renderer::AddToFrame(selectedTile->neighbor[SOUTH_WEST]);

			selectedTile->neighbor[SOUTH_EAST]->CheckTileOverlap(drawBox);
			Renderer::AddToFrame(selectedTile->neighbor[SOUTH_EAST]);

			lastPos = selectedTile;
			state = 0;
			break;
		}
	}
}

void TileSelector::MoveTo(Tile * destination)
{
	// Prevents the same tile from being selected over and over again
	if(destination != nullptr && destination != selectedTile && doneMoving){
		doneMoving = false;

		this->selectedTile = destination;
	}
}

uint8_t StartScreen::MainMenu()
{
	const uint8_t highlightColor = 0x32;
	const uint8_t arrowColor = 0xD8;
	ps2_inputs_t	menuInput;
	ps2_inputs_t	keyState;
	rectangle_t TextBox;
	uint8_t textColor;
	bool redrawMenu = false;

	char textMenuOptions[4][10] = {{"New Game"},
								{"Load Game"},
								{"Tutorial"},
								{"Credits"}};

	TextBox.topleft.x = 325;
	TextBox.topleft.y = 250;
	TextBox.botright.x = 475;
	TextBox.botright.y = 460;

	InitPS2Controller(0);

	for(int i = 0; i < 2; i++){
		Draw_Line(TextBox.topleft.x, TextBox.topleft.y, TextBox.botright.x, TextBox.topleft.y, BLANC);
		Draw_Line(TextBox.topleft.x, TextBox.topleft.y, TextBox.topleft.x, TextBox.botright.y, BLANC);
		Draw_Line(TextBox.botright.x, TextBox.topleft.y, TextBox.botright.x, TextBox.botright.y, BLANC);
		Draw_Line(TextBox.topleft.x, TextBox.botright.y, TextBox.botright.x, TextBox.botright.y, BLANC);

		DrawStrConst(textMenuOptions[0], TextBox.topleft.x + 30, 300, 16, highlightColor);
		DrawStrConst(textMenuOptions[1], TextBox.topleft.x + 30, 330, 16, BLANC);
		DrawStrConst(textMenuOptions[2], TextBox.topleft.x + 30, 360, 16, BLANC);
		DrawStrConst(textMenuOptions[3], TextBox.topleft.x + 30, 390, 16, BLANC);

		DrawMenuArrow(TextBox.topleft.x + 5, TextBox.topleft.y + 57 + selection * 30, arrowColor, 0);

		MemorySwap();
	}

	while(1){
		if(LPC_TIMER0->TC >= 1600000){
			LPC_TIMER0->TC = 0;
			PollController(0, &menuInput);
		}

		if(menuInput.Button.Down){
			if(keyState.Button.Down == RELEASED){
				keyState.Button.Down = PRESSED;
				redrawMenu = true;
				selection++;
				selection %= 4;
			}
		}
		else{
			keyState.Button.Down = RELEASED;
		}
		if(menuInput.Button.Up){
			if(keyState.Button.Up == RELEASED){
				keyState.Button.Up = PRESSED;
				redrawMenu = true;
				if(selection > 0)
					selection--;
				else
					selection = 3;
			}
		}
		else{
			keyState.Button.Up = RELEASED;
		}
		if(menuInput.Button.X){
			if(selection == 0)
				return selection;
		}

		if(redrawMenu){
			redrawMenu = false;

			for(int i = 0; i < 2; i++){
				for(int j = 0; j < 4; j++){
					if(j == selection){
						textColor = highlightColor;
						DrawMenuArrow(TextBox.topleft.x + 5, TextBox.topleft.y + 57 + j * 30, arrowColor, 0);
					}
					else{
						textColor = BLANC;
						DrawMenuArrow(TextBox.topleft.x + 5, TextBox.topleft.y + 57 + j * 30, NOIR, 0);
					}
					DrawStrConst(textMenuOptions[j], TextBox.topleft.x + 30, 300 + j * 30, 16, textColor);
				}
				MemorySwap();
			}
		}
	}

	return selection;
}

