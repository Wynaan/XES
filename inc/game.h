#ifndef GAME_H_
#define GAME_H_
#include <stdint.h>
#include <utility>
#include "xav_data.h"

class Game{
public:
	static resources_t Resources, ProdPer5;
	static std::pair<uint8_t, uint8_t> getAvailableFieldsInfo(uint8_t type);
	static void AddUsedField(uint8_t buildType);
	static void InitNewGame();
	static void LoadGame();
};



#endif /* GAME_H_ */
