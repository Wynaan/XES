#include <stdint.h>
#include <utility>
#include "game.h"
#include <cstring>

uint8_t AvailableResourceFields[5];
uint8_t MaxResourceFields[5];
resources_t Game::Resources = {0};
resources_t Game::ProdPer5 = {0};

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

void Game::LoadGame()
{

}




