#ifndef M_OBJ_H_
#define M_OBJ_H_
#include "interfaces.h"
#include "xav_data.h"
#include "engine.h"
#include "gui.h"
#include "map.h"

class Default : public IMapObject{
public:
	void OnUserInteract();
	Default(){}
	~Default(){}
};

class City : public IMapObject{
public:
	void OnUserInteract();
	City(){}
	~City(){}
};

class Buildable : public IMapObject, public IUpdateable{
	UserInterface * GUI = nullptr;
	const resources_t * cost;
	int8_t exitProcessCount;
	Tile * sourceTile;
	bool exit;
	bool canBuild;
	bool built;
	bool validInput;
	tile_asset_e buildType;
public:
	void Tick();
	void OnUserInteract();

	Buildable(Tile * selfPtr, tile_asset_e type) :
		sourceTile(selfPtr),
		buildType(type){}
	~Buildable(){}
};

class ResourceMine : public IMapObject, public IUpdateable{
	UserInterface * GUI = nullptr;
	int8_t exitProcessCount;
	uint8_t level;
	tile_asset_e buildType;
	resources_t cost;
	bool exit;
	bool canUpgrade;
	bool upgraded;
	bool validInput;
public:
	void Tick();
	void OnUserInteract();

	ResourceMine(tile_asset_e type) :
		buildType(type){
		level = 1;
	}
	~ResourceMine(){}
};



#endif /* M_OBJ_H_ */
