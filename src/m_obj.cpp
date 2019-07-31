#include "m_obj.h"
#include "gui.h"
#include "ps2controller.h"
#include "board.h"
#include "engine.h"
#include "text.h"
#include "image.h"
#include "user_interface.h"
#include "production.h"
#include "game.h"
#include <cstdlib>
#include <cmath>
#include "benchmarklogger.h"

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

const char buildName[5][12] =
{
	"  Sawmill ",
	"   Farm   ",
	"  Quarry  ",
	"Iron Mine ",
	"Gold Mine "
};

void Default::OnUserInteract()
{

}

void City::OnUserInteract()
{

}

void Buildable::OnUserInteract()
{
	const coords_pair_t UIPos = { .x = 240, .y = 150};
	const coords_pair_t assetImage1 = { .x = (int16_t)(UIPos.x + 60), .y = (int16_t)(UIPos.y + 20)};
	const coords_pair_t assetImage2 = { .x = (int16_t)(UIPos.x + 220), .y = (int16_t)(UIPos.y + 20)};
	const coords_pair_t cancelButton = { .x = (int16_t)(UIPos.x + 20), .y = (int16_t)(UIPos.y + 250)};
	const coords_pair_t buildButton = { .x = (int16_t)(UIPos.x + 180), .y = (int16_t)(UIPos.y + 250)};
	std::pair<uint8_t, uint8_t> availableFields = Game::getAvailableFieldsInfo(buildType);
	keyState.digitalState = 0;
	static char asciiProd[5][4];
	exit = false;
	validInput = false;
	built = false;
	exitProcessCount = -1;

	memset(asciiProd, 0, sizeof(asciiProd));

	switch(buildType){
	case SAWMILL:
		cost = &sawmillBaseCost;
		strcpy(asciiProd[1], "+ ");
		itoa(tier1Production[0], &asciiProd[1][2], 10);
		break;
	case FARM:
		cost = &farmBaseCost;
		strcpy(asciiProd[0], "+ ");
		itoa(tier1Production[0], &asciiProd[0][2], 10);
		break;
	case QUARRY:
		cost = &quarryBaseCost;
		strcpy(asciiProd[2], "+ ");
		itoa(tier2Production[0], &asciiProd[2][2], 10);
		break;
	case IRON_MINE:
		cost = &ironMineBaseCost;
		strcpy(asciiProd[3], "+ ");
		itoa(tier3Production[0], &asciiProd[3][2], 10);
		break;
	case GOLD_MINE:
		cost = &goldMineBaseCost;
		strcpy(asciiProd[4], "+ ");
		itoa(tier4Production[0], &asciiProd[4][2], 10);
		break;
	default:
		break;
	}

	if(std::get<0>(availableFields) && *cost < Game::Resources)
		canBuild = true;
	else
		canBuild = false;

	/* UI size = 320 x 300 */
	GUI = new UserInterface(UIPos.x, UIPos.y, 14, 13);

	GUI->AddElement(new Asset(buildType, assetImage1));
	GUI->AddElement(new Asset(buildType, assetImage2));

	GUI->AddElement(new Button(Button::RED, cancelButton, 4));
	GUI->AddElement(new Button(Button::GREEN, buildButton, 4));

	GUI->AddElement(new Text(buildName[buildType - 9], assetImage1.x + 50, assetImage1.y + 15, 16, 0xFF));
	GUI->AddElement(new Text("Available :", UIPos.x + 60, UIPos.y + 80, 10, 0xFF));
	GUI->AddElement(new Text("/", UIPos.x + 150, UIPos.y + 80, 10, 0xFF));
	GUI->AddElement(new Text(availableFields.first, UIPos.x + 140, UIPos.y + 80, 10, availableFields.first > 0 ? VERT : ROUGE));
	GUI->AddElement(new Text(availableFields.second, UIPos.x + 160, UIPos.y + 80, 10, BLANC));
	GUI->AddElement(new Text("Cost", UIPos.x + 140, UIPos.y + 120, 10, ORANGE));
	GUI->AddElement(new Text("Production", UIPos.x + 220, UIPos.y + 120, 10, 0x70));
	GUI->AddElement(new Text("Food", UIPos.x + 60, UIPos.y + 140, 10, 0xFF));
	GUI->AddElement(new Text(cost->food, UIPos.x + 140, UIPos.y + 140, 10, cost->food < Game::Resources.food ? BLANC : ROUGE));
	GUI->AddElement(new Text(asciiProd[0], UIPos.x + 220, UIPos.y + 140, 10, VERT));
	GUI->AddElement(new Text("Wood", UIPos.x + 60, UIPos.y + 160, 10, 0xFF));
	GUI->AddElement(new Text(cost->wood, UIPos.x + 140, UIPos.y + 160, 10, cost->wood < Game::Resources.wood ? BLANC : ROUGE));
	GUI->AddElement(new Text(asciiProd[1], UIPos.x + 220, UIPos.y + 160, 10, VERT));
	GUI->AddElement(new Text("Stone", UIPos.x + 60, UIPos.y + 180, 10, 0xFF));
	GUI->AddElement(new Text(cost->stone, UIPos.x + 140, UIPos.y + 180, 10, cost->stone < Game::Resources.stone ? BLANC : ROUGE));
	GUI->AddElement(new Text(asciiProd[2], UIPos.x + 220, UIPos.y + 180, 10, VERT));
	GUI->AddElement(new Text("Iron", UIPos.x + 60, UIPos.y + 200, 10, 0xFF));
	GUI->AddElement(new Text(cost->iron, UIPos.x + 140, UIPos.y + 200, 10, cost->iron < Game::Resources.iron ? BLANC : ROUGE));
	GUI->AddElement(new Text(asciiProd[3], UIPos.x + 220, UIPos.y + 200, 10, VERT));
	GUI->AddElement(new Text("Gold", UIPos.x + 60, UIPos.y + 220, 10, 0xFF));
	GUI->AddElement(new Text(cost->gold, UIPos.x + 140, UIPos.y + 220, 10, cost->gold < Game::Resources.gold ? BLANC : ROUGE));
	GUI->AddElement(new Text(asciiProd[4], UIPos.x + 220, UIPos.y + 220, 10, VERT));
	GUI->AddElement(new Text("Cancel", cancelButton.x + 10, cancelButton.y + 12, 16, BLANC));
	GUI->AddElement(new Text("Build", buildButton.x + 10, buildButton.y + 12, 16, canBuild ? BLANC : NOIR));

	GUI->AddElement(new Image(ps2_circle, cancelButton.x + 90, cancelButton.y + 8, 24, 24, BLANC));
	GUI->AddElement(new Image(ps2_cross, buildButton.x + 90, buildButton.y + 8, 24, 24, BLANC));

	TickEngine::PushStack(this, Stack::L2);
}

void Buildable::Tick()
{
	PollController(0, &userInput);

	if(!userInput.Button.digitalState)
		validInput = true;

	if(validInput){
		if(userInput.Button.X && keyState.X == RELEASED){
			keyState.X = PRESSED;
			if(canBuild && !built){
				built = true;
				sourceTile->AddAsset(buildType);
				Game::AddUsedField(buildType);
				Game::Resources -= *cost;

				switch(buildType){
				case SAWMILL:
					Game::ProdPer5.wood += tier1Production[0];
					break;
				case FARM:
					Game::ProdPer5.food += tier1Production[0];
					break;
				case QUARRY:
					Game::ProdPer5.stone += tier2Production[0];
					break;
				case IRON_MINE:
					Game::ProdPer5.iron += tier3Production[0];
					break;
				case GOLD_MINE:
					Game::ProdPer5.gold += tier4Production[0];
					break;
				default:
					break;
				}
				exit = true;
			}
		}
		else
			keyState.X = RELEASED;

		if(userInput.Button.O && keyState.O == RELEASED){
			keyState.O = PRESSED;
			exit = true;
		}
		else
			keyState.O = RELEASED;
	}

	/* Redraws the tile if it is modified */
	if(exitProcessCount > 0){
		exitProcessCount--;
		if(built){
			sourceTile->ResetDrawBox();
			Renderer::AddToFrame(sourceTile);
			sourceTile->neighbor[SOUTH_WEST]->CheckTileOverlap(sourceTile->drawBox);
			Renderer::AddToFrame(sourceTile->neighbor[SOUTH_WEST]);
			sourceTile->neighbor[SOUTH_EAST]->CheckTileOverlap(sourceTile->drawBox);
			Renderer::AddToFrame(sourceTile->neighbor[SOUTH_EAST]);
		}
	}
	/* Removes this map object from the update stack */
	else if(exitProcessCount == 0){
		TickEngine::PopStack(Stack::L2);

		// deletes and replace the map object
		if(built)
			sourceTile->SetNewMapObject();
	}
	/* Makes sure the GUI erases itself before returning */
	if(exitProcessCount == -1){
		if(!GUI->ProcessGraphics(exit)){
			exitProcessCount = 2;
			delete GUI;
		}
	}
}

void ResourceMine::OnUserInteract()
{
	const coords_pair_t UIPos = { .x = 240, .y = 150};
	const coords_pair_t assetImage1 = { .x = (int16_t)(UIPos.x + 60), .y = (int16_t)(UIPos.y + 20)};
	const coords_pair_t assetImage2 = { .x = (int16_t)(UIPos.x + 220), .y = (int16_t)(UIPos.y + 20)};
	const coords_pair_t cancelButton = { .x = (int16_t)(UIPos.x + 20), .y = (int16_t)(UIPos.y + 250)};
	const coords_pair_t upgradeButton = { .x = (int16_t)(UIPos.x + 160), .y = (int16_t)(UIPos.y + 250)};
	static char asciiProd[2][5][6];
	keyState.digitalState = 0;
	exit = false;
	validInput = false;
	upgraded = false;
	exitProcessCount = -1;

	memset(asciiProd, 0, sizeof(asciiProd));

	switch(buildType){
	case SAWMILL:
		cost = sawmillBaseCost;
		itoa(tier1Production[level - 1], asciiProd[0][1], 10);
		strcpy(asciiProd[1][1], "+ ");
		itoa(tier1Production[level] - tier1Production[level - 1], &asciiProd[1][1][2], 10);
		break;
	case FARM:
		cost = farmBaseCost;
		itoa(tier1Production[level - 1], asciiProd[0][0], 10);
		strcpy(asciiProd[1][0], "+ ");
		itoa(tier1Production[level] - tier1Production[level - 1], &asciiProd[1][0][2], 10);
		break;
	case QUARRY:
		cost = quarryBaseCost;
		itoa(tier2Production[level - 1], asciiProd[0][2], 10);
		strcpy(asciiProd[1][2], "+ ");
		itoa(tier2Production[level] - tier2Production[level - 1], &asciiProd[1][2][2], 10);
		break;
	case IRON_MINE:
		cost = ironMineBaseCost;
		itoa(tier1Production[level - 1], asciiProd[0][3], 10);
		strcpy(asciiProd[1][3], "+ ");
		itoa(tier3Production[level] - tier3Production[level - 1], &asciiProd[1][3][2], 10);
		break;
	case GOLD_MINE:
		cost = goldMineBaseCost;
		itoa(tier1Production[level - 1], asciiProd[0][4], 10);
		strcpy(asciiProd[1][4], "+ ");
		itoa(tier4Production[level] - tier4Production[level - 1], &asciiProd[1][4][2], 10);
		break;
	default:
		break;
	}

	cost *= pow(2, level);

	if(cost < Game::Resources && level < 10)
		canUpgrade = true;
	else
		canUpgrade = false;

	/* UI size = 320 x 300 */
	GUI = new UserInterface(UIPos.x, UIPos.y, 14, 13);

	GUI->AddElement(new Asset(buildType, assetImage1));
	GUI->AddElement(new Asset(buildType, assetImage2));

	GUI->AddElement(new Button(Button::RED, cancelButton, 4));
	GUI->AddElement(new Button(Button::GREEN, upgradeButton, 5));

	GUI->AddElement(new Text(buildName[buildType - 9], assetImage1.x + 50, assetImage1.y + 15, 16, 0xFF));
	GUI->AddElement(new Text("Current Level :", UIPos.x + 60, UIPos.y + 80, 10, 0xFF));
	GUI->AddElement(new Text(level, UIPos.x + 160, UIPos.y + 74, 16, JAUNE));
	GUI->AddElement(new Text("Cost", UIPos.x + 140, UIPos.y + 120, 10, ORANGE));
	GUI->AddElement(new Text("Production", UIPos.x + 220, UIPos.y + 120, 10, 0x70));
	GUI->AddElement(new Text("Food", UIPos.x + 60, UIPos.y + 140, 10, 0xFF));
	GUI->AddElement(new Text(cost.food, UIPos.x + 140, UIPos.y + 140, 10, cost.food < Game::Resources.food ? BLANC : ROUGE));
	GUI->AddElement(new Text(asciiProd[0][0], UIPos.x + 220, UIPos.y + 140, 10, BLANC));
	GUI->AddElement(new Text(asciiProd[1][0], UIPos.x + 250, UIPos.y + 140, 10, VERT));
	GUI->AddElement(new Text("Wood", UIPos.x + 60, UIPos.y + 160, 10, 0xFF));
	GUI->AddElement(new Text(cost.wood, UIPos.x + 140, UIPos.y + 160, 10, cost.wood < Game::Resources.wood ? BLANC : ROUGE));
	GUI->AddElement(new Text(asciiProd[0][1], UIPos.x + 220, UIPos.y + 160, 10, BLANC));
	GUI->AddElement(new Text(asciiProd[1][1], UIPos.x + 250, UIPos.y + 160, 10, VERT));
	GUI->AddElement(new Text("Stone", UIPos.x + 60, UIPos.y + 180, 10, 0xFF));
	GUI->AddElement(new Text(cost.stone, UIPos.x + 140, UIPos.y + 180, 10, cost.stone < Game::Resources.stone ? BLANC : ROUGE));
	GUI->AddElement(new Text(asciiProd[0][2], UIPos.x + 220, UIPos.y + 180, 10, BLANC));
	GUI->AddElement(new Text(asciiProd[1][2], UIPos.x + 250, UIPos.y + 180, 10, VERT));
	GUI->AddElement(new Text("Iron", UIPos.x + 60, UIPos.y + 200, 10, 0xFF));
	GUI->AddElement(new Text(cost.iron, UIPos.x + 140, UIPos.y + 200, 10, cost.iron < Game::Resources.iron ? BLANC : ROUGE));
	GUI->AddElement(new Text(asciiProd[0][3], UIPos.x + 220, UIPos.y + 200, 10, BLANC));
	GUI->AddElement(new Text(asciiProd[1][3], UIPos.x + 250, UIPos.y + 200, 10, VERT));
	GUI->AddElement(new Text("Gold", UIPos.x + 60, UIPos.y + 220, 10, 0xFF));
	GUI->AddElement(new Text(cost.gold, UIPos.x + 140, UIPos.y + 220, 10, cost.gold < Game::Resources.gold ? BLANC : ROUGE));
	GUI->AddElement(new Text(asciiProd[0][4], UIPos.x + 220, UIPos.y + 220, 10, BLANC));
	GUI->AddElement(new Text(asciiProd[1][4], UIPos.x + 250, UIPos.y + 220, 10, VERT));
	GUI->AddElement(new Text("Cancel", cancelButton.x + 10, cancelButton.y + 12, 16, BLANC));
	GUI->AddElement(new Text("Upgrade", upgradeButton.x + 10, upgradeButton.y + 12, 16, canUpgrade ? BLANC : NOIR));

	GUI->AddElement(new Image(ps2_circle, cancelButton.x + 90, cancelButton.y + 8, 24, 24, BLANC));
	GUI->AddElement(new Image(ps2_cross, upgradeButton.x + 110, upgradeButton.y + 8, 24, 24, BLANC));

	TickEngine::PushStack(this, Stack::L2);
}

void ResourceMine::Tick()
{
	PollController(0, &userInput);

	if(!userInput.Button.digitalState)
		validInput = true;

	if(validInput){
		if(userInput.Button.X && keyState.X == RELEASED){
			keyState.X = PRESSED;
			if(canUpgrade && !upgraded){
				upgraded = true;
				Game::Resources -= cost;

				switch(buildType){
				case SAWMILL:
					Game::ProdPer5.wood += (tier1Production[level] - tier1Production[level-1]);
					break;
				case FARM:
					Game::ProdPer5.food += (tier1Production[level] - tier1Production[level-1]);
					break;
				case QUARRY:
					Game::ProdPer5.stone += (tier2Production[level] - tier2Production[level-1]);
					break;
				case IRON_MINE:
					Game::ProdPer5.iron += (tier3Production[level] - tier3Production[level-1]);
					break;
				case GOLD_MINE:
					Game::ProdPer5.gold += (tier4Production[level] - tier4Production[level-1]);
					break;
				default:
					break;
				}
				level++;

				Game::SaveHandler.memoryBuffer.mapBuilds[this->memorySaveID].level = this->level;

				exitProcessCount = 0;
			}
		}
		else
			keyState.X = RELEASED;

		if(userInput.Button.O && keyState.O == RELEASED){
			keyState.O = PRESSED;
			exit = true;
		}
		else
			keyState.O = RELEASED;
	}

	if(exitProcessCount == -1 || !GUI->isReady()){
		// Makes sure the GUI is erased if user wants to exit
		/* Draws the GUI */
		if(!GUI->ProcessGraphics(exit)){
			exitProcessCount = 0;
			delete GUI;
		}
	}
	else if(exitProcessCount == 0){
		/* Removes this map object from the update stack */
		TickEngine::PopStack(Stack::L2);
		if(upgraded){
			this->OnUserInteract();
		}
	}
}

ResourceMine::ResourceMine(Tile * selfPtr, tile_asset_e type, uint8_t level, uint8_t saveID) :
	sourceTile(selfPtr),
	level(level),
	buildType(type){
	if(saveID != 255)
		memorySaveID = saveID;
	else
		memorySaveID = Game::SaveHandler.memoryBuffer.nbOfBuilds++;
	Game::SaveHandler.memoryBuffer.mapBuilds[memorySaveID].level = level;
	Game::SaveHandler.memoryBuffer.mapBuilds[memorySaveID].type = buildType;
	Game::SaveHandler.memoryBuffer.mapBuilds[memorySaveID].coords = sourceTile->getTileCoords();
}

