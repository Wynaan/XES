#ifndef MAP_H_
#define MAP_H_
#include "renderer.h"
#include <stdint.h>
#include <cstdlib>
#include "xav_data.h"
#include "interfaces.h"

#define SSP_DATA (*((uint32_t *)0x40088008))

#define GRID_WIDTH	(18 + 2)
#define GRID_HEIGHT (15 + 2)

class Tile;
class WorldManager;
/***********************************************************************************/
class Asset : public IRender{
	friend Tile;
protected:
	const uint8_t * pixelArray;
	tile_asset_e type;
	coords_pair_t pos;
	uint8_t width, height;

public:
	void Draw(rectangle_t drawBox);

	Asset(tile_asset_e type, coords_pair_t tilePos);
};
/***********************************************************************************/
class Tile : public IRender{
friend WorldManager;
private:
	coords_pair_t tile; // tile position in the grid
	coords_pair_t center; // absolute position for entities on the tile
	coords_pair_t pos; // absolute position of the upper left corner
	uint8_t width, height;
	tile_type_e type;
	Asset * asset;
	bool isDefined = false;

	/* Implements IRender::Draw() */
	void Draw(rectangle_t drawBox);
	/* Draw the whole tile */
	void DrawTile();
	/* Set object properties based on type and position */
	void SetAttributes(uint8_t t_x,
						uint8_t t_y,
						tile_type_e type,
						tile_asset_e asset = NONE);
public:
	bool hasAsset = false;
	bool isObstacle = false;
	bool isWater = false;
	IInteract * object;
	Tile * neighbor[6] = {nullptr};
	/* returns tile type */
	tile_type_e getType() const;
	/* returns tile X axis grid position */
	uint8_t getTileX() const;
	/* returns tile Y axis grid position */
	uint8_t getTileY() const;
	/* returns tile position in the grid */
	coords_pair_t getTileCoords() const;
	/* returns absolute tile coordinates */
	coords_pair_t getPos() const;
	/* returns center tile coords adjusted for tile type */
	coords_pair_t getEntityCenterCoords() const;
	/* Sets the draw box coordinates back to the full size of the tile */
	void ResetDrawBox();
	/* Check for tile overlap with hitboxRect, and place result in IRender::drawBox if there is */
	bool CheckTileOverlap(rectangle_t hitboxRect);
	/* Modifies properties of the tile, adds it to the render list */
	void AddAsset(tile_asset_e asset);
	/* Delete the interactable object and create a new one */
	void SetNewMapObject(uint8_t level = 1);
};
/***********************************************************************************/
class WorldManager{
private:
	Tile tileArray[GRID_WIDTH][GRID_HEIGHT];
	uint8_t gridWidth;
	uint8_t gridHeight;

	/* Define pointers to adjacent tiles in the argument array */
	void SetNeighbors();
public:
	static Tile * worldArray[GRID_WIDTH][GRID_HEIGHT];

	void SetMapAsCurrentView();
	/* Initialize tile array for scene to be displayed*/
	void InitMap(tile_type_e * mapAttributeList, uint8_t * mapAssetList, uint8_t width, uint8_t height);
	/* Load every defined tile */
	void LoadMap();
};
#endif /* MAP_H_ */
