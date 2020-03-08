#include "board.h"
#include "animation_sprites.h"
#include "entities.h"
#include <stdint.h>
#include "geometry.h"
#include "ps2controller.h"
#include "engine.h"
#include "game.h"

#define SSP_DATA (*((uint32_t *)0x40088008))

void Player::Draw_Sprite(const uint8_t * sprite, uint16_t x, uint16_t y)
{
	for(uint16_t i = 0; i < 768; i++){
		if(sprite[i] != 0xFF){
			uint32_t PixelData = 0x80000000 + ((((((i / 24) + y) * 0x420) + ((i % 24) + x)) << 8) + sprite[i]);
			SSP_DATA = ((PixelData >> 16) & 0xFFFF);
			SSP_DATA = (PixelData & 0xFFFF);
			for(uint8_t i = 0; i < 9; i++);
		}
	}
}

uint16_t Player::getX() const
{
	return(currentCoords.x);
}

uint16_t Player::getY() const
{
	return(currentCoords.y);
}

tile_direction_e Player::getCurrentDirection() const
{
	return this->direction;
}

tile_direction_e Player::getSelectorDirection() const
{
	for(uint8_t i = 0; i < 6; i++){
		if(this->selectedTile == this->currentTile->neighbor[(tile_direction_e)i])
			return (tile_direction_e)i;
	}
	return this->getCurrentDirection();
}

void Player::ChangeDirection(tile_direction_e newDirection)
{
	if(this->direction != newDirection)
		hasChangedSprite = 1;

	this->direction = newDirection;
}

bool Player::ValidateTarget(Tile * target)
{
	if(target->getType() == V)
		return false;
	if(target->isObstacle)
		return false;
	if(target->isWater){
		if(target->hasAsset)
			return true;
		else
			return false;
	}

	return true;
}

void Player::SetSelector(Tile * neighborTile)
{
	if(neighborTile->getType() != V){
		selectedTile = neighborTile;
		selectCursor.MoveTo(selectedTile);
	}
}

bool Player::CheckForArrival()
{
	coords_pair_t dest = this->targetTile->getEntityCenterCoords();

	if(dest.y == currentCoords.y && dest.x == currentCoords.x){
		this->currentTile = this->targetTile;
		this->isMoving = false;
		this->selectedTile = currentTile->neighbor[cursorDirection];
		showSelector = popSelector;
		return true;
	}
	else
		return false;
}

void Player::InitMove(Tile * target)
{
	if(ValidateTarget(target)){
		this->targetTile = target;
		coords_pair_t dest = targetTile->getEntityCenterCoords();
		this->isMoving = true;

		popSelector = showSelector;
		showSelector = false;

		this->m_dx = dest.x - currentCoords.x;
		this->m_dy = dest.y - currentCoords.y;
		this->m_err = m_dy / 2;
		int8_t slope = m_dy / m_dx;

		if(slope > 0 && dest.x > currentCoords.x){
			this->ChangeDirection(SOUTH_EAST);
			m_xstep = 1;
		}
		else if(slope == 0 && dest.x > currentCoords.x){
			this->ChangeDirection(EAST);
			m_xstep = 2;
		}
		else if(slope < 0 && dest.x > currentCoords.x){
			this->ChangeDirection(NORTH_EAST);
			m_xstep = 1;
		}
		else if(slope > 0 && dest.x < currentCoords.x){
			this->ChangeDirection(NORTH_WEST);
			m_xstep = -1;
		}
		else if(slope == 0 && dest.x < currentCoords.x){
			this->ChangeDirection(WEST);
			m_xstep = -2;
		}
		else if(slope < 0 && dest.x < currentCoords.x){
			this->ChangeDirection(SOUTH_WEST);
			m_xstep = -1;
		}
	}
}

void Player::Move()
{
	coords_pair_t dest = this->targetTile->getEntityCenterCoords();

	if(m_dx > 0)
		m_err -= m_dx;
	else
		m_err += m_dx;

	if(m_err < 0)
	{
		if(dest.x != currentCoords.x)
			currentCoords.x += m_xstep;
		if(m_dy > 0)
			m_err += m_dy;
		else
			m_err -= m_dy;
	}

	if(dest.y > currentCoords.y)
		currentCoords.y++;
	else if(dest.y < currentCoords.y)
		currentCoords.y--;

	/* Add 2 pixel around the player sprite */
	drawBox.topleft.x = this->currentCoords.x - 14;
	drawBox.topleft.y = this->currentCoords.y - 26;
	drawBox.botright.x = this->currentCoords.x + 14;
	drawBox.botright.y = this->currentCoords.y + 10;

	this->CycleAnimationSprite();
}

void Player::CycleAnimationSprite()
{
	this->frame++;
	if(frame == 3)
		frame = 0;
}

void Player::ProcessMovement()
{
	if(this->isMoving){
		this->Move();
		this->CheckForArrival();
	}
}

void Player::Draw(rectangle_t DrawBox)
{
	Draw_Sprite(Player1[this->direction][this->frame], currentCoords.x - 12, currentCoords.y - 24); // center character at 3/4 of body height
}

void Player::ProcessGraphics()
{
	if(this->isMoving){
		/* Check for any void tiles in the player's surroundings */
		for(int i = 0; i < 6; i++){
			if(currentTile->neighbor[(tile_direction_e)i]->getType() == V){
				if(currentTile->neighbor[(tile_direction_e)i]->CheckTileOverlap(this->drawBox))
					Renderer::AddToCallback(currentTile->neighbor[(tile_direction_e)i]);
			}
		}
		/* Check the player's surrounding tiles for overlap, top to bottom */
		if(currentTile->neighbor[NORTH_WEST]->getType() != V){
			if(currentTile->neighbor[NORTH_WEST]->CheckTileOverlap(this->drawBox))
				Renderer::AddToCallback(currentTile->neighbor[NORTH_WEST]);
		}
		if(currentTile->neighbor[NORTH_EAST]->getType() != V){
			if(currentTile->neighbor[NORTH_EAST]->CheckTileOverlap(this->drawBox))
				Renderer::AddToCallback(currentTile->neighbor[NORTH_EAST]);
		}
		if(currentTile->neighbor[WEST]->getType() != V){
			if(currentTile->neighbor[WEST]->CheckTileOverlap(this->drawBox))
				Renderer::AddToCallback(currentTile->neighbor[WEST]);
		}
		if(currentTile->CheckTileOverlap(this->drawBox)){
			Renderer::AddToCallback(currentTile);
		}
		if(currentTile->neighbor[EAST]->getType() != V){
			if(currentTile->neighbor[EAST]->CheckTileOverlap(this->drawBox))
				Renderer::AddToCallback(currentTile->neighbor[EAST]);
		}
		if(currentTile->neighbor[SOUTH_WEST]->getType() != V){
			if(currentTile->neighbor[SOUTH_WEST]->CheckTileOverlap(this->drawBox))
				Renderer::AddToCallback(currentTile->neighbor[SOUTH_WEST]);
		}
		if(currentTile->neighbor[SOUTH_EAST]->getType() != V){
			if(currentTile->neighbor[SOUTH_EAST]->CheckTileOverlap(this->drawBox))
				Renderer::AddToCallback(currentTile->neighbor[SOUTH_EAST]);
		}
	}
	else{
		if(showSelector)
			ChangeDirection(cursorDirection);
	}

	selectCursor.ProcessGraphics(this->showSelector);

	if(hasChangedSprite){
		currentTile->neighbor[NORTH_WEST]->CheckTileOverlap(this->drawBox);
		Renderer::AddToFrame(currentTile->neighbor[NORTH_WEST]);
		currentTile->neighbor[NORTH_EAST]->CheckTileOverlap(this->drawBox);
		Renderer::AddToFrame(currentTile->neighbor[NORTH_EAST]);
		currentTile->CheckTileOverlap(this->drawBox);
		Renderer::AddToFrame(currentTile);

		if(hasChangedSprite == 2)
			hasChangedSprite = 0;
		else
			hasChangedSprite++;
	}

	Renderer::AddToFrame(this);
}

void Player::ProcessInput()
{
	static ps2_buttons_t keyState;

	PollController(controllerID, &playerInput);

	if(!this->isMoving){
		if(playerInput.JoystickLeftY < 112){
			if(playerInput.JoystickLeftX > 132)
				this->InitMove(this->currentTile->neighbor[NORTH_EAST]);
			else if(playerInput.JoystickLeftX < 124)
				this->InitMove(this->currentTile->neighbor[NORTH_WEST]);
		}
		else if(playerInput.JoystickLeftY > 144){
			if(playerInput.JoystickLeftX > 132)
				this->InitMove(this->currentTile->neighbor[SOUTH_EAST]);
			else if(playerInput.JoystickLeftX < 124)
				this->InitMove(this->currentTile->neighbor[SOUTH_WEST]);
		}
		else if(playerInput.JoystickLeftY < 136 && playerInput.JoystickLeftY > 120){
			if(playerInput.JoystickLeftX < 1)
				this->InitMove(this->currentTile->neighbor[WEST]);
			else if(playerInput.JoystickLeftX > 244)
				this->InitMove(this->currentTile->neighbor[EAST]);
		}
	}

	if(playerInput.JoystickRightY < 112){
		if(playerInput.JoystickRightX > 132)
			cursorDirection = NORTH_EAST;
		else if(playerInput.JoystickRightX < 124)
			cursorDirection = NORTH_WEST;
	}
	else if(playerInput.JoystickRightY > 144){
		if(playerInput.JoystickRightX > 132)
			cursorDirection = SOUTH_EAST;
		else if(playerInput.JoystickRightX < 124)
			cursorDirection = SOUTH_WEST;
	}
	else if(playerInput.JoystickRightY < 136 && playerInput.JoystickLeftY > 120){
		if(playerInput.JoystickRightX < 12)
			cursorDirection = WEST;
		else if(playerInput.JoystickRightX > 244)
			cursorDirection = EAST;
	}

	if(!isMoving && showSelector)
		this->SetSelector(currentTile->neighbor[cursorDirection]);

	if(playerInput.Button.O && keyState.O == RELEASED){
		if(!this->isMoving) // Disallow selector render while moving
			this->showSelector = !this->showSelector;
		keyState.O = PRESSED;
	}
	if(!playerInput.Button.O){
		keyState.O = RELEASED;
	}

	if(playerInput.Button.X && keyState.X == RELEASED && showSelector){
		keyState.X = PRESSED;
		selectedTile->object->OnUserInteract();
	}
	if(!playerInput.Button.X){
		keyState.X = RELEASED;
	}

	if(playerInput.Button.Start && keyState.Start == RELEASED){
		keyState.X = PRESSED;
		Game::SaveHandler.OnUserInteract();
	}
	if(!playerInput.Button.Start){
		keyState.Start = RELEASED;
	}
}

void Player::InitController()
{
	InitPS2Controller(this->controllerID);

	/* Do a few dummy pollings to prevent false movement triggers */
  	do{
		if(LPC_TIMER0->TC >= 1600000){
			LPC_TIMER0->TC = 0;
			PollController(controllerID, &playerInput);
		}
	}while(playerInput.JoystickLeftY > 135 || playerInput.JoystickLeftY < 120);

  	/* Add a delay to prevent main loop from double-polling after init */
	while(LPC_TIMER0->TC < 1600000);
}

IUpdate* Player::getMenuUpdateHandle()
{
	return (IUpdate *)this->selectedTile->object;
}

void Player::Tick()
{
	this->ProcessInput();
	this->ProcessGraphics();
	this->ProcessMovement();
}

Player::Player(Tile * initialTilePosition, uint8_t controllerID)
{
	this->currentCoords = initialTilePosition->getEntityCenterCoords();
	this->currentTile = initialTilePosition;
	this->targetTile = this->currentTile;
	this->selectedTile = this->currentTile->neighbor[SOUTH_EAST];
	this->cursorDirection = SOUTH_EAST;
	this->controllerID = controllerID;

	this->frame = 0;
	this->direction = SOUTH;

	this->isMoving = false;

	/* Add 2 pixel around the player sprite */
	drawBox.topleft.x = this->currentCoords.x - 14;
	drawBox.topleft.y = this->currentCoords.y - 26;
	drawBox.botright.x = this->currentCoords.x + 14;
	drawBox.botright.y = this->currentCoords.y + 10;

	selectCursor.MoveTo(selectedTile);
}
/********************************************************************************************/
void Ball::Update_Position()
{
	this->x += this->vx;
	this->y += this->vy;

	if(this->Shadow != nullptr)
		this->Shadow->Update_Position();
}

void Ball::Check_Collision()
{
	if(this->x + this->r >= SCREEN_WIDTH)
		this->vx = -this->vx;
	if(this->y + this->r >= SCREEN_HEIGHT)
		this->vy = -this->vy;
	if(this->x - this->r <= 5)
		this->vx = -this->vx;
	if(this->y - this->r <= 5)
		this->vy = -this->vy;

	if(this->Shadow != nullptr)
		this->Shadow->Check_Collision();
}

void Ball::Draw()
{
	Draw_Circle(this->Shadow->x, this->Shadow->y, this->Shadow->r, this->Shadow->color);
	Draw_Circle(this->x, this->y, this->r, this->color);
}



