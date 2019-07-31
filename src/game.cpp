#include <stdint.h>
#include <utility>
#include "game.h"
#include "lpc_xav.h"
#include "interfaces.h"
#include <cstring>

extern _I2C I2C_Bus;

uint8_t AvailableResourceFields[5];
uint8_t MaxResourceFields[5];
resources_t Game::Resources = {0};
resources_t Game::ProdPer5 = {0};
save_cell_t Game::loadBuffer = {0};

SaveManager Game::SaveHandler;

std::pair<uint8_t, uint8_t> Game::getAvailableFieldsInfo(uint8_t buildType)
{
	std::pair<uint8_t, uint8_t> temp = std::make_pair(AvailableResourceFields[buildType - 9], MaxResourceFields[buildType - 9]);
	return temp;
}

void Game::AddUsedField(uint8_t buildType)
{
	AvailableResourceFields[buildType - 9] -= 1;
}

void Game::InitNewGame()
{
	for(uint8_t i = 0; i < 5; i++){
		AvailableResourceFields[i] = 1;
		MaxResourceFields[i] = 1;
	}

	Resources.food = 45298;
  	Resources.wood = 32187;
  	Resources.stone = 12342;
  	Resources.iron = 23009;
  	Resources.gold = 7765;

  	ProdPer5.food = 14;
  	ProdPer5.wood = 22;
  	ProdPer5.stone = 18;
  	ProdPer5.iron = 44;
  	ProdPer5.gold = 6;
}

void Game::LoadGame(uint8_t slot)
{
	uint16_t address = 0x2000 * slot;
	uint8_t * _data = (uint8_t *)&loadBuffer;

	I2C_Bus.New_Sequence(0xA0);
	I2C_Bus.Add_Byte(address >> 8);
	I2C_Bus.Add_Byte(0x00);
	I2C_Bus.Send_Sequence();

	while(I2C_Bus.Busy);

	I2C_Bus.New_Sequence(0xA1, sizeof(save_cell_t));
	I2C_Bus.Send_Sequence();

	while(I2C_Bus.Busy);

	for(uint16_t j = 0; j < sizeof(save_cell_t); j++){
		*(_data++) = I2C_Bus.rx_buf[j];
	}

	for(uint8_t i = 0; i < 5; i++){
		AvailableResourceFields[i] = 1;
		MaxResourceFields[i] = 1;
	}

	Resources = loadBuffer.Resources;
  	ProdPer5 = loadBuffer.Prod;

  	for(uint8_t i = 0; i < loadBuffer.nbOfBuilds; i++){
  		coords_pair_t c = loadBuffer.mapBuilds[i].coords;

  		WorldManager::worldArray[c.x + 1][c.y + 1]->AddAsset(loadBuffer.mapBuilds[i].type);
  		WorldManager::worldArray[c.x + 1][c.y + 1]->SetNewMapObject(loadBuffer.mapBuilds[i].level);

  		AddUsedField(loadBuffer.mapBuilds[i].type);
  	}
}




