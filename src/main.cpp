/*
===============================================================================
 Name        : main.cpp
 Author      : Xavier Dufour
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
#include "chip.h"
#include "board.h"
#include <cr_section_macros.h>
#include "lpc_xav.h"
#include "entities.h"
#include "map.h"
#include "gui.h"
#include "renderer.h"
#include "engine.h"
#include "interfaces.h"
#include "game.h"
#include "benchmarklogger.h"

/* Memory select line, wired to 74LVC74 of the DDC-860 */
LPC_IO CS(P0, 16, GPIO, OUT);

/* SSP0 bus, wired to the SPI decoder of the DDC-860 */
LPC_IO SCK0(P0, 15, Fct0x2, OUT, Float);
LPC_IO MOSI0(P0, 18, Fct0x2, OUT, Pulldown);

/* SSP1 bus, wired to the Playstation2 controllers */
LPC_IO SCK1(P0, 7, Fct0x2, OUT, Float);
LPC_IO MISO1(P0, 8, Fct0x2, IN, Pullup);
LPC_IO MOSI1(P0, 9, Fct0x2, OUT, Pulldown);

/* Attention line for the Playstation2 controllers */
LPC_IO ControllerSelect(P0, 6, GPIO, OUT);
LPC_IO ControllerACK(P0, 5, GPIO, IN, Float);

/* VGA VSync input line from the DDC-860 */
LPC_IO VSYNC(P0, 17, GPIO, IN, Float);

SSP DDC860_Bus((uint32_t *)0x40088000, 10, 14);
SSP PS2Controller_Bus((uint32_t *)0x40030000, 20, 15);

uint8_t MenuSelection = 0;
extern tile_type_e newWorldTiles[340];
extern uint8_t newWorldAssets[340];

/**************** Game engine structure ******************
 *
 * 		IRender 						IMapObject
 * 		|								|
 * 		|	WorldManager				|---Default
 * 		|	|							|
 * 		|---|---Tile					|---ResourceMine
 * 		|		|						|	|
 * 		|		|---Asset				|	|---UserInterface
 * 		|		|						|
 * 		|		|---IMapObject			|---Buildable
 * 		|								|	|
 * 		|---Entity						|	|---UserInterface
 * 		|	|							|
 * 		|	|---Player					|---City
 * 		|	|	|
 * 		|	|	|---Selector
 * 		|	|
 * 		|	|---Enemy
 * 		|		|
 * 		|		|---Golem
 * 		|		|
 * 		|		|---Skeleton
 * 		|
 * 		|---Selector
 * 		|
 * 		|---UserInterface
 *
 *
 **********************************************************/

int main(void) {
	Board_LED_Set(0, false);
  	SystemCoreClockUpdate();
    Board_Init();
    /* Initialize Timer0 at 96MHz peripheral clock */
    TMR0.Init(CLK0);
    /* CPSDVSR = 4 for 24MHz clock, 16-bit transfers, CPOL 0, CPHA 0 */
    DDC860_Bus.Init(4, 16, 0, 0);
    /* CPSDVSR = 240, SCR = 1 for 200KHz clock, 8-bit transfers, CPOL 1, CPHA 1 */
    PS2Controller_Bus.Init(240, 8, 1, 1);
    LPC_SSP1->CR0 |= (1 << 8);
    /* Initialize controller attention line high */
    ControllerSelect = 1;
    /********************* End of system initialization *************************/
    /* Initialize both memories */
    ClearScreen();
    /* Create loading screen */
    StartScreen Init;
    /* Enter loading screen */
    MenuSelection = Init.MainMenu();
    /* Create new game */
    Game::InitNewGame();
    /* Create main world map tile array */
    WorldManager World;
    /* Initialize each tile in the main map */
    World.InitMap(newWorldTiles, newWorldAssets, 20, 17);
    /* Create resource bar at the top of the screen */
    InfoDisplay TopDisplay(32, 5);
  	/* Add all the map tiles and assets to the render queue */
  	World.LoadMap();
  	/* Set the static map pointer to current map */
  	World.SetMapAsCurrentView();
  	/* Create player and initialize position */
	Player P1(WorldManager::worldArray[7][10], 0);
  	/* Draw the resource bar with corresponding values */
  	TopDisplay.Load();
  	/* Renders the initial world scene in both SRAMs */
  	Renderer::Init();
  	/* Initialize PS2 controller */
  	P1.InitController();
  	/* Build the update priority queue */
  	TickEngine::PushStack(&TopDisplay, Stack::L1);
  	TickEngine::PushStack(&P1, Stack::L2);
    /*************** End of game initialization **********************/
    while(1){
    	TickEngine::Tick();
    	Renderer::Update();
    }
    return 0 ;
}
