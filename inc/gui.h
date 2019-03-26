#ifndef GUI_H_
#define GUI_H_
#include <stdint.h>
#include "xav_data.h"
#include "renderer.h"
#include "engine.h"
#include "map.h"
#include <deque>

class Tile;
class UserInterface;

class TiledImage{
protected:
	coords_pair_t pos;
	uint8_t sWidth, sHeight;
	uint8_t tWidth, tHeight;
};

class Background : public TiledImage, public IRender{
public:
	void Draw(rectangle_t drawBox);
	Background(coords_pair_t position, uint8_t sectionWidth, uint8_t sectionHeight);
};

class Button : public TiledImage, public IRender{
	const uint8_t * pImage[3];
public:
	typedef enum{GREEN, RED}btn_color_e;

	void Draw(rectangle_t drawBox);
	Button(btn_color_e color, coords_pair_t position, uint8_t sectionWidth);
};

class UserInterface{
protected:
	uint8_t sWidth, sHeight;	// size of the background table, in 20px wide section
	coords_pair_t pos;			// position of the origin
	uint8_t state;
	Background * bg = nullptr;
	std::deque<IRender*> items;
public:
	uint8_t selection = 0;

	void AddElement(IRender* item);
	bool ProcessGraphics(bool exit);
	UserInterface(int16_t x, int16_t y, uint8_t sectionWidth, uint8_t sectionHeight);
	~UserInterface(){ delete bg;}
};

class InfoDisplay : public IRender, public IUpdateable{
private:
	Background * bg = nullptr;
	coords_pair_t pos;

	void Draw(rectangle_t drawBox);
public:
	/* Draws the graphic box with it's icons */
	void Load();
	/* Update displayed values */
	void Tick();
	/* Change the amount of resources gained every 5 seconds */
	void ChangeProduction(resources_t newValues);
	/* Set the initial values to be displayed on game startup */
	void SetInitialValues(resources_t total, resources_t gain);
	/* Initialize new resource bar */
	InfoDisplay(uint8_t x, uint8_t y);
};

class Inventory : public UserInterface{
private:
	uint8_t nbSlotsTotal;
	uint8_t nbSlotsEmpty;
public:
	void Fetch();
	void Update();
};

class TileSelector : public IRender{
private:
	bool doneMoving = true;
	Tile * selectedTile = nullptr;
	uint8_t state = 0;
	/* Draw hexagon around selected tile */
	void Draw(rectangle_t drawBox);
public:
	void ProcessGraphics(bool showSelector);
	void MoveTo(Tile * destination);
};

class StartScreen{
	uint8_t selection = 0;
public:
	uint8_t MainMenu();
};

#endif /* GUI_H_ */
