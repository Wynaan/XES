#include "tileset.h"
#include "assets.h"
#include "map.h"
#include "m_obj.h"
#include "entities.h"
#include <stdint.h>

#define PLAYER_Y_GRID_OFFSET	3
#define TILE_WIDTH				40
#define TILE_HEIGHT				53

tile_type_e newWorldTiles[340] =
{
	V, V, V, V, V, V, V, V, V, V, V, V, V, V, V, V, V, V, V, V,
	V,G1,G1,G1,G1,G1,G1,G1,G1,G1,G1,G5,G5,W9,W9,W9,W9,W9,W9, V, // row 0
	V,G1,G1,G1,G9,G1,G2,G1,G1,G1,G5,G5,W9,W1,W8,W9,W2,W9,W9, V, // row 1
	V,G1,G1,G1,G6,G1,G7,G1,G1,G1,G5,G5,W3,W9,W1,W9,W10,W9,W9,V, // row 2
	V,G1,G9,G1,G11,G1,G1,G1,G1,G1,G5,G5,W9,W9,W11,W9,W4,W9,W9,V,// row 3
	V,G1,G1,G1,G1,G8,G1,G1,G1,G1,G5,G5,W9,W9,W9,W5,W9,W9,W9, V, // row 4
	V,G1,G1,G1,G3,G1,G1,G4,G1,G5,G5,G5,W9,W9,W9,W9,W9,W9,W9, V, // row 5
	V,G1,G1,G1,G1,G1,G1,G1,G1,G1,G5,G5,G5,W9,W9,W9,G5,G5,G5, V, // row 6
	V,G1,G1,G1,G1,G1,G1,G1,G1,G1,G5,G5,G5,G5,G5,G5,G5,G5,G5, V, // row 7
	V,G1,G1,G1,G1,G1,G1,G1,G1,G1,G1,G5,G5,G5,G5,G5,G5,S9,S9, V, // row 8
	V,G1,G1,G1,G1,G1,G1,G1,G1,G1,G5,G5,G5,G5,S9,S9,S9,S2,S2, V, // row 9
	V,G1,G1,G1,G1,G1,G9,G1,G1,G5,G5,G5,G5,S9,S9,S8,S9,S10,S9,V, // row 10
	V,G1,G1,G1,G1,G1,G1,G1,G5,G5,G5,S9,S9,S11,S9,S9,S1,S9,S9,V, // row 11
	V,G1,G1,G1,G1,G1,G1,G1,G5,G5,G5,S9,S4,S9,S6,S6,S9,S9,S9, V, // row 12
	V,G1,G1,G1,G1,G1,G1,G5,G5,G5,S9,S9,S9,S9,S5,S5,S9,S9,S9, V, // row 13
	V,G1,G1,G9,G1,G1,G1,G5,G5,G5,S9,S9,S9,S9,S5,S9,S9,S9,S9, V, // row 14
	V, V, V, V, V, V, V, V, V, V, V, V, V, V, V, V, V, V, V, V
};

uint8_t newWorldAssets[340] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 0
	0, 0, B_CASTLE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 1
	0, 0, 0, 0, 0, 0, 0, 0, SAWMILL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 2
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 3
	0, 0, GOLD_MINE, 0, 0, 0, 0, 0, 0, 0, BRIDGE, BRIDGE, 0, 0, 0, 0, 0, 0, 0, 0, // row 4
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 5
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 6
	0, 0, 0, 0, 0, 0, FACTORY1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 7
	0, 0, IRON_MINE, 0, 0, FARM, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 8
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 9
	0, 0, 0, 0, 0, W_CASTLE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 10
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 11
	0, 0, 0, 0, 0, 0, 0, 0, BRIDGE, BRIDGE, BRIDGE, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 12
	0, 0, TOWN_HALL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 13
	0, 0, 0, 0, 0, QUARRY, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // row 14
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void MemorySwap();

void Asset::Draw(rectangle_t drawBox)
{
	uint16_t arraySize = width * height;

	for(uint16_t i = 0; i < arraySize; i++){
		if(this->pixelArray[i] != 0x01){
			uint32_t PixelData = 0x80000000 + (((((i / width) + pos.y) * 0x420) + (i % width) + pos.x) << 8) + pixelArray[i];
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t j = 0; j < 9; j++);
		}
	}
}

Asset::Asset(tile_asset_e t, coords_pair_t tilePos)
{
	this->type = t;

	if(type == BRIDGE){
		this->pixelArray = Bridge;
		this->width = 40;
		this->height = 24;
		this->pos.y = tilePos.y + ((TILE_HEIGHT - height) / 2) - 8;
	}
	else if(type >= TOWN_HALL && type <= FACTORY2){
		this->pixelArray = City[t - 2];
		this->width = 38;
		this->height = 34;
		this->pos.y = tilePos.y + ((TILE_HEIGHT - height) / 2) - 5;
	}
	else if(type == SAWMILL){
		this->pixelArray = Sawmill;
		this->width = 35;
		this->height = 31;
		this->pos.y = tilePos.y + ((TILE_HEIGHT - height) / 2) - 5;
	}
	else if(type == FARM){
		this->pixelArray = Farm;
		this->width = 38;
		this->height = 28;
		this->pos.y = tilePos.y + ((TILE_HEIGHT - height) / 2) - 5;
	}
	else if(type >= QUARRY && type <= GOLD_MINE){
		this->pixelArray = Mine[t - 11];
		this->width = 32;
		this->height = 30;
		this->pos.y = tilePos.y + ((TILE_HEIGHT - height) / 2) - 5;
	}

	this->pos.x = tilePos.x + ((TILE_WIDTH - width) / 2);
}
/*********************************************************************************/
tile_type_e Tile::getType() const
{
	return type;
}

uint8_t Tile::getTileX() const
{
	return tile.x;
}

uint8_t Tile::getTileY() const
{
	return tile.y;
}

coords_pair_t Tile::getTileCoords() const
{
	return tile;
}

coords_pair_t Tile::getPos() const
{
	return pos;
}

coords_pair_t Tile::getEntityCenterCoords() const
{
	coords_pair_t adjusted;

	adjusted.x = this->center.x;
	if(this->isWater){
		if(this->hasAsset && this->asset->type == BRIDGE)
			/* Add offset to compensate higher water tile y coordinate */
			adjusted.y = this->center.y - 10;
	}
	else{
		adjusted.y = this->center.y;
	}
	return adjusted;
}

void Tile::ResetDrawBox()
{
	drawBox.topleft.x = pos.x;
	drawBox.topleft.y = pos.y;
	drawBox.botright.x = pos.x + width - 1;
	drawBox.botright.y = pos.y + height - 1;
}

bool Tile::CheckTileOverlap(rectangle_t hitboxRect)
{
	drawBox.topleft.x = hitboxRect.topleft.x > pos.x ? hitboxRect.topleft.x : pos.x;
	drawBox.topleft.y = hitboxRect.topleft.y > pos.y ? hitboxRect.topleft.y : pos.y;
	drawBox.botright.x = hitboxRect.botright.x < (pos.x + (width - 1)) ? hitboxRect.botright.x : (pos.x + (width - 1));
	drawBox.botright.y = hitboxRect.botright.y < (pos.y + (height - 1)) ? hitboxRect.botright.y : (pos.y + (height - 1));

	/* if the bottom corner is not higher than top corner */
	if(drawBox.botright.x < drawBox.topleft.x || drawBox.botright.y < drawBox.topleft.y)
		return false;
	else
		return true;
}

void Tile::Draw(rectangle_t drawBox)
{
	if(this->isDefined){
		uint32_t PixelAddress;
		uint8_t PixelData;
		uint8_t rectWidth = drawBox.botright.x - drawBox.topleft.x;
		coords_pair_t k = drawBox.topleft; // vertical and horizontal iterator

		PixelAddress = 0x00800000 + (k.y * 0x420) + k.x;
		while(1){
			if(this->hasAsset && // If an asset is defined on this tile
			(k.y >= asset->pos.y && k.y < asset->pos.y + asset->height) && // if y counter is within the asset boundaries
			(k.x >= asset->pos.x && k.x < asset->pos.x + asset->width) && // if x counter is within the asset boundaries
			((PixelData = asset->pixelArray[((k.y - asset->pos.y) * asset->width) + (k.x - asset->pos.x)]) != 0x01)){ // If array index is a valid pixel
				for(uint8_t i = 0; i < 1; i++);
				SSP_DATA = ((PixelAddress >> 8) & 0xFFFF);
				SSP_DATA = ((PixelAddress & 0xFF) << 8) | PixelData;
			}
			else if((PixelData = HexTile[this->type][((k.y - pos.y) * TILE_WIDTH) + k.x - pos.x]) != 0x01){
				for(uint8_t i = 0; i < 1; i++);
				SSP_DATA = ((PixelAddress >> 8) & 0xFFFF);
				SSP_DATA = ((PixelAddress & 0xFF) << 8) | PixelData;
			}
			if(k.x == drawBox.botright.x){ // if x is at the end of the rectangle, reset x and increment y
				if(k.y == drawBox.botright.y)
					break;
				else{
					k.y++;
					k.x = drawBox.topleft.x;
					PixelAddress += 0x420 - rectWidth;
				}
			}
			else{
				k.x++;
				PixelAddress++;
			}
		}
	}
}

void Tile::DrawTile()
{
	if(this->isDefined && this->type != V){
		for(uint16_t i = 0; i < 2120; i++){
			if(HexTile[this->type][i] != 0x01){
				uint32_t PixelData = 0x80000000 + (((((i / 40) + pos.y) * 0x420) + ((i % 40) + pos.x)) << 8) + HexTile[this->type][i];
				SSP_DATA = ((PixelData >> 16) & 0xFFFF);
				SSP_DATA = (PixelData & 0xFFFF);
				for(uint8_t i = 0; i < 9; i++);
			}
		}
	}
}

void Tile::SetAttributes(uint8_t t_x, uint8_t t_y, tile_type_e t, tile_asset_e a)
{
	this->type = t;

	switch(type){
	case G1:
	case G9:
	case S9:
	case S2:
	case W9:
	case W6:
	case G2:
	case S1:
	case W5:
		this->isObstacle = false;
		break;
	case G5:
	case S3:
	case W7:
		this->isWater = true;
		break;
	default:
		this->isObstacle = true;
		break;
	}

	/* Add mapObject depending on what is available */
	switch(type){
	case G3:
	case G4:
		object = new Buildable(this, SAWMILL);
		break;
	case G7:
		object = new Buildable(this, QUARRY);
		break;
	case G9:
		object = new Buildable(this, FARM);
		break;
	case W1:
		object = new Buildable(this, IRON_MINE);
		break;
	case S6:
	case S8:
		object = new Buildable(this, GOLD_MINE);
		break;
	default:
		object = new Default;
		break;
	}

	this->tile.x = t_x;
	this->tile.y = t_y;

	this->width = TILE_WIDTH;
	this->height = TILE_HEIGHT;

	/* 35 pixel per tile row, + 12 pixel initial offset */
	pos.y = tile.y * 35 + 12;
	if(this->isWater)
		pos.y += 6;
	/* 1 pixel removed to height so center always upper left if 2x2 center, + entities y offset*/
	center.y = pos.y + ((height - 1) / 2) - PLAYER_Y_GRID_OFFSET;

	/* 40 pixel per tile column, - 8 pixel initial offset*/
	pos.x = (tile.x * 40) - 8;

	/* Offset x by half a column if on even (odd on visible map) column for hexagonal tiling */
	if(this->tile.y % 2 == 0)
		pos.x += 20;

	/* Offset all the tile coordinates by 1 to adjust for void tiles */
	this->tile.x -= 1;
	this->tile.y -= 1;

	/* 1 pixel removed to width so center always upper left if 2x2 center */
	center.x = pos.x + ((width - 1) / 2);

	/* bounding box initialization, defaults to the whole size of the tile */
	drawBox.topleft.x = pos.x;
	drawBox.topleft.y = pos.y;
	drawBox.botright.x = pos.x + width - 1;
	drawBox.botright.y = pos.y + height - 1;

	if(a){
		this->asset = new Asset(a, pos);
		this->hasAsset = true;
		if(this->asset->type != BRIDGE)
			this->isObstacle = true;
	}

	this->isDefined = true;
}

void Tile::AddAsset(tile_asset_e asset)
{
	if(type <= G11)
		this->type = G1;
	else if(type >= W1)
		this->type = W9;
	else
		this->type = S9;

	this->asset = new Asset(asset, this->pos);
	this->hasAsset = true;
	this->isObstacle = true;
}

void Tile::SetNewMapObject()
{
	delete object;

	if(asset->type >= SAWMILL && asset->type <= GOLD_MINE)
		object = new ResourceMine(asset->type);
}
/******************************************************************************/
Tile * WorldManager::worldArray[GRID_WIDTH][GRID_HEIGHT];

void WorldManager::SetMapAsCurrentView()
{
	for(uint8_t y = 0; y < GRID_HEIGHT; y++){
		for(uint8_t x = 0; x < GRID_WIDTH; x++){
			worldArray[x][y] = &this->tileArray[x][y];
		}
	}
}

void WorldManager::SetNeighbors()
{
	for(uint8_t y = 0; y < this->gridHeight; y++){
		for(uint8_t x = 0; x < this->gridWidth; x++){
			if((x % 2 == 1 && y % 2 == 1) || (x % 2 == 0 && y % 2 == 1)){ // Even row, even column OR Even row, odd column
				if(y < gridHeight - 1)
					tileArray[x][y].neighbor[SOUTH_EAST] = &tileArray[x][y + 1];
				else
					tileArray[x][y].neighbor[SOUTH_EAST] = &tileArray[17][14];

				if(x > 0 && y > 0)
					tileArray[x][y].neighbor[NORTH_WEST] = &tileArray[x - 1][y - 1];
				else
					tileArray[x][y].neighbor[NORTH_WEST] = &tileArray[17][14];

				if(y > 0)
					tileArray[x][y].neighbor[NORTH_EAST] = &tileArray[x][y - 1];
				else
					tileArray[x][y].neighbor[NORTH_EAST] = &tileArray[17][14];

				if(x > 0 && y < gridHeight - 1)
					tileArray[x][y].neighbor[SOUTH_WEST] = &tileArray[x - 1][y + 1];
				else
					tileArray[x][y].neighbor[SOUTH_WEST] = &tileArray[17][14];
			}
			else if((x % 2 == 1 && y % 2 == 0) || (x % 2 == 0 && y % 2 == 0)){ // Odd row, even column OR Odd row, odd column
				if(x < gridWidth - 1 && y < gridHeight - 1)
					tileArray[x][y].neighbor[SOUTH_EAST] = &tileArray[x + 1][y + 1];
				else
					tileArray[x][y].neighbor[SOUTH_EAST] = &tileArray[17][14];

				if(y > 0)
					tileArray[x][y].neighbor[NORTH_WEST] = &tileArray[x][y - 1];
				else
					tileArray[x][y].neighbor[NORTH_WEST] = &tileArray[17][14];

				if(x < gridWidth - 1 && y > 0)
					tileArray[x][y].neighbor[NORTH_EAST] = &tileArray[x + 1][y - 1];
				else
					tileArray[x][y].neighbor[NORTH_EAST] = &tileArray[17][14];

				if(y < gridHeight - 1)
					tileArray[x][y].neighbor[SOUTH_WEST] = &tileArray[x][y + 1];
				else
					tileArray[x][y].neighbor[SOUTH_WEST] = &tileArray[17][14];
			}

			if(x < gridWidth - 1)
				tileArray[x][y].neighbor[EAST] = &tileArray[x + 1][y];
			else
				tileArray[x][y].neighbor[EAST] = &tileArray[17][14];

			if(x > 0)
				tileArray[x][y].neighbor[WEST] = &tileArray[x - 1][y];
			else
				tileArray[x][y].neighbor[WEST] = &tileArray[17][14];
		}
	}
}

void WorldManager::InitMap(tile_type_e * mapAttributeList, uint8_t * mapAssetList, uint8_t width, uint8_t height)
{
	this->gridWidth = width;
	this->gridHeight = height;

	for(uint8_t y = 0; y < height; y++){
		for(uint8_t x = 0; x < width; x++){
			tileArray[x][y].SetAttributes(x, y, mapAttributeList[y * width + x], (tile_asset_e)mapAssetList[y * width + x]);
		}
	}

	SetNeighbors();
}

void WorldManager::LoadMap()
{
	// Draw every tile with it's asset if applicable
	for(int j = 1; j < this->gridHeight - 1; j++){ // y axis
		for(int k = 1; k < this->gridWidth - 1; k++){ // x axis
			Renderer::AddToFrame(&tileArray[k][j]);
		}
	}
}
