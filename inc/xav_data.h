#ifndef XAV_DATA_H_
#define XAV_DATA_H_
#include <stdint.h>
#include <string>

enum class ReturnCode {FAILURE, SUCCESS, RUNNING};

typedef enum{G1, // Plains 1
			G2, // Small rocks
			G3, // Forest 1
			G4, // Forest 2
			G5, // Water
			G6, // Snow hills
			G7, // Rocky hills
			G8, // Pond
			G9, // Plains 2
			G10, // Wooden shed
			G11, // Tower
			S1, // Small rocks
			S2, // Plains 2
			S3, // Water
			S4, // Pond
			S5, // Trees 1
			S6, // Sand dunes
			S7, // Trees 2
			S8, // Hills
			S9, // Plains 1
			S10, // Wooden shed
			S11, // Tower
			W1, // Rocky hills
			W2, // Ice pond
			W3, // Forest 1
			W4, // Forest 2
			W5, // Small rocks
			W6, // Plains 2
			W7, // Icy water
			W8, // Snowy hills
			W9, // Plains 1
			W10, // Wooden shed
			W11, // Tower
			V // Null map
			}tile_type_e;

typedef enum{NONE,
			BRIDGE,
			TOWN_HALL,
			B_CASTLE1,
			B_CASTLE2,
			W_CASTLE1,
			W_CASTLE2,
			FACTORY1,
			FACTORY2,
			SAWMILL,
			FARM,
			QUARRY,
			IRON_MINE,
			GOLD_MINE
			}tile_asset_e;

typedef enum{
	SOUTH_EAST,
	EAST,
	NORTH_EAST,
	NORTH_WEST,
	WEST,
	SOUTH_WEST,
	NORTH,
	SOUTH
	}tile_direction_e;

typedef enum {
	RELEASED,
	PRESSED
	}keystate_e;

typedef struct{
	int16_t x, y;
}coords_pair_t;

typedef struct{
	uint16_t x0, y0;
	uint16_t x1, y1;
}line_coords_t;

typedef struct{
	coords_pair_t topleft,
				botright;
}rectangle_t;

struct resources_t{
	uint32_t food;
	uint32_t wood;
	uint32_t stone;
	uint32_t iron;
	uint32_t gold;

	resources_t& operator+=(const resources_t& other){
		this->food += other.food;
		this->wood += other.wood;
		this->stone += other.stone;
		this->iron += other.iron;
		this->gold += other.gold;

		return *this;
	}
	resources_t& operator-=(const resources_t& other){
		this->food -= other.food;
		this->wood -= other.wood;
		this->stone -= other.stone;
		this->iron -= other.iron;
		this->gold -= other.gold;

		return *this;
	}
	bool operator<(const resources_t other) const{
		if(this->food > other.food) return false;
		if(this->wood > other.wood) return false;
		if(this->stone > other.stone) return false;
		if(this->iron > other.iron) return false;
		if(this->gold > other.gold) return false;

		return true;
	}
	bool operator!=(const resources_t& other) const{
		if(this->food != other.food) return true;
		if(this->wood != other.wood) return true;
		if(this->stone != other.stone) return true;
		if(this->iron != other.iron) return true;
		if(this->gold != other.gold) return true;

		return false;
	}
	resources_t& operator*=(uint16_t mul){
		this->food *= mul;
		this->wood *= mul;
		this->stone *= mul;
		this->iron *= mul;
		this->gold *= mul;

		return *this;
	}

};

typedef struct{
	char menuName[12];
	uint8_t level;
	resources_t costToNextLevel;
	resources_t currentProduction;
	resources_t	nextLevelProduction;
}resource_mine_info_t;

typedef struct{
	std::string gameName;
	tile_type_e mainWorldTiles[340];
	uint8_t mainWorldAssets[340];
	coords_pair_t p1TilePosition;
	coords_pair_t p2TilePosition;

}save_game_data_t;


#endif /* XAV_DATA_H_ */
