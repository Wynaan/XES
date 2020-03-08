#ifndef GAME_H_
#define GAME_H_
#include <stdint.h>
#include <utility>
#include "xav_data.h"
#include "savemanager.h"

class Game{
public:
	static SaveManager SaveHandler;
	static resources_t Resources, ProdPer5;
	static save_cell_t loadBuffer;
	static std::pair<uint8_t, uint8_t> getAvailableFieldsInfo(uint8_t type);
	static void AddUsedField(uint8_t buildType);
	static void InitNewGame();
	static void LoadGame(uint8_t slot);
};



#endif /* GAME_H_ */
