#ifndef ENTITIES_H_
#define ENTITIES_H_
#include <stdint.h>
#include "map.h"
#include "xav_data.h"
#include "gui.h"
#include "ps2controller.h"
#include "renderer.h"
#include "interfaces.h"
#include "engine.h"

class Player : public IRender, public IUpdateable{
private:
	bool isMoving = false;				// already-in-movement flag
	bool showSelector = false;			// render the selector flag
	bool popSelector = false;			// flag for re-render of the selector upon movement
	bool interact = false;
	ps2_inputs_t playerInput;			// container for all the controller button states
	coords_pair_t currentCoords; 		// for pixel-wise x and y
	tile_direction_e direction; 		// sprite direction
	tile_direction_e cursorDirection;	// cursor direction
	uint8_t hasChangedSprite = false;	// flag for idle sprite change
	uint8_t frame; 						// sprite animation counter
	uint8_t controllerID;				// controller ID for input polling
	int8_t m_err = 0, m_xstep = 0;		// bresenham slope calculation values
	int8_t m_dx = 0, m_dy = 0; 			// distance to target tile
	rectangle_t drawBox;				// hitbox used for erasal during movement
	Tile * targetTile; 					// tile player is currently moving to if moving
	Tile * selectedTile; 				// tile the player has the action selector on */
	Tile * currentTile;					// tile player is currently on */
	TileSelector selectCursor;

	/* Draws the determined sprite from the array at the argument coordinates */
	void Draw_Sprite(const uint8_t * sprite, uint16_t x, uint16_t y);
	/* Implements IRender::Draw() */
	void Draw(rectangle_t drawBox);
	/* Increment frame variable value in a loop from 0 to 2 */
	void CycleAnimationSprite();
	/* Change player direction to argument value*/
	void ChangeDirection(tile_direction_e newDirection);
	/* Check if the player is allowed to go on this tile */
	bool ValidateTarget(Tile * target);
	/* Checks if coordinates are equal to target tile and update tile position accordinly */
	bool CheckForArrival();
	/* Returns player's current direction */
	tile_direction_e getCurrentDirection() const;
	/* Returns the selected tile direction relative to player */
	tile_direction_e getSelectorDirection() const;
	/* Returns player's absolute X axis coordinate */
	uint16_t getX() const;
	/* Returns player's absolute Y axis coordinate */
	uint16_t getY() const;
	/* Initialize player movement to selected tile */
	void InitMove(Tile * target);
	/* Updates player coordinates using determined slope and direction */
	void Move();
	/* Select a neighbor tile*/
	void SetSelector(Tile * neighborTile);
	/* Moves the player until it has reached it's destination */
	void ProcessMovement();
	/* Redraws the background tiles the player hitbox overlaps */
	void ProcessGraphics();
	/* Verify and take actions according to player's controller */
	void ProcessInput();
public:
	/* Initialize controller corresponding to the player's controller ID */
	void InitController();
	/* */
	IUpdateable* getMenuUpdateHandle();
	/* Updates all the different process for the player functions */
	void Tick();

	Player(Tile * initialTilePosition, uint8_t controllerID);
};

class Ball{
public:
	int16_t x, y, r;
	int16_t vx, vy;
	uint8_t color;
	Ball * Shadow = nullptr;

	void Update_Position();
	void Check_Collision();
	void Draw();

	Ball(int16_t b_x, int16_t b_y, int16_t b_r, int16_t b_vx, int16_t b_vy, uint8_t b_c)
		:x(b_x),
		y(b_y),
		r(b_r),
		vx(b_vx),
		vy(b_vy),
		color(b_c)
	{}
};

#endif /* ENTITIES_H_ */
