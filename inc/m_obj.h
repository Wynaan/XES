#ifndef M_OBJ_H_
#define M_OBJ_H_
#include "interfaces.h"
#include "xav_data.h"
#include "engine.h"
#include "gui.h"
#include "map.h"

class Default : public IInteract{
public:
	void OnUserInteract();
	Default(){}
	~Default(){}
};

class City : public IInteract{
public:
	void OnUserInteract();
	City(){}
	~City(){}
};

class Buildable : public IInteract, public IUpdate{
	UserInterface * GUI = nullptr;
	const resources_t * cost;
	int8_t exitProcessCount;
	Tile * sourceTile;
	tile_asset_e buildType;
	bool exit;
	bool canBuild;
	bool built;
	bool validInput;
public:
	void Tick();
	void OnUserInteract();

	Buildable(Tile * selfPtr, tile_asset_e type) :
		sourceTile(selfPtr),
		buildType(type){}
	~Buildable(){}
};

class ResourceMine : public IInteract, public IUpdate{
	UserInterface * GUI = nullptr;
	int8_t exitProcessCount;
	Tile * sourceTile;
	uint8_t level;
	tile_asset_e buildType;
	uint8_t memorySaveID;
	resources_t cost;
	bool exit;
	bool canUpgrade;
	bool upgraded;
	bool validInput;
public:
	void Tick();
	void OnUserInteract();

	ResourceMine(Tile * selfPtr,
				tile_asset_e type,
				uint8_t level,
				uint8_t saveID = 255);
	~ResourceMine(){}
};



#endif /* M_OBJ_H_ */
