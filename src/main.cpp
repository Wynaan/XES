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

extern void *__end_of_heap;

/* Memory select line, wired to 74LVC74 of the DDC-860 */
LPC_IO CS(P0, 16, GPIO, OUT);

/* SSP0 bus, wired to the SPI decoder of the DDC-860 */
LPC_IO SCK0(P0, 15, Fct0x2, OUT, Float);
LPC_IO MOSI0(P0, 18, Fct0x2, OUT, Pulldown);

/* VGA VSync input line from the DDC-860 */
LPC_IO VSYNC(P0, 17, GPIO, IN, Float);

/* SSP1 bus, wired to the Playstation2 controllers */
LPC_IO SCK1(P0, 7, Fct0x2, OUT, Float);
LPC_IO MISO1(P0, 8, Fct0x2, IN, Pullup);
LPC_IO MOSI1(P0, 9, Fct0x2, OUT, Pulldown);

/* Control signals for the Playstation2 controllers */
LPC_IO Controller1Select(P0, 6, GPIO, OUT);
LPC_IO Controller2Select(P0, 5, GPIO, OUT);

/* I2C0 Bus for memory card data transfer */
LPC_IO SDA(P0, 27, Fct0x1, IN, Float, Open_Drain);
LPC_IO SCL(P0, 28, Fct0x1, IN, Float, Open_Drain);

/* Signal line for single-step detection of the memory card */
LPC_IO MEM_DETECT(P0, 22, GPIO, IN, Float);

/* UART3 for DLPUSB232-R debug console */
LPC_IO TXD3(P0, 0, Fct0x2); // Console output UART
LPC_IO RXD3(P0, 1, Fct0x2);

SSP DDC860_Bus((uint32_t *)0x40088000, 10, 14);
SSP PS2Controller_Bus((uint32_t *)0x40030000, 20, 15);

extern _I2C I2C_Bus;
extern tile_type_e newWorldTiles[340];
extern uint8_t newWorldAssets[340];

uint32_t dynamicMemoryUsed = 0;

int main(void) {
  	SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, true);
    /* Initialize Timer0 at 96MHz peripheral clock */
    TMR0.Init(CLK0);
    /* Initialize UART3 at 115200 baudrate for console debug */
    Console.Init(115200);
    Console.Set_Transmission(8, Disabled);
    /* Initialize I2C0 at 400KHz clock */
    I2C_Bus.Init(Master, I2C_400KHZ);
    /* CPSDVSR = 4 for 24MHz clock, 16-bit transfers, CPOL 0, CPHA 0 */
    DDC860_Bus.Init(4, 16, 0, 0);
    /* CPSDVSR = 240, SCR = 1 for 200KHz clock, 8-bit transfers, CPOL 1, CPHA 1 */
    PS2Controller_Bus.Init(240, 8, 1, 1);
    LPC_SSP1->CR0 |= (1 << 8);
    /* Initialize controller attention line high */
    Controller1Select = 1;
    Controller2Select = 1;
    /********************* End of system initialization *************************/
    /* Initialize both memories */
    ClearScreen();
    /* Create loading screen */
    StartScreen Init;
    /* Create main world map tile array */
    WorldManager World;
    /* Initialize each tile in the main map */
    World.InitMap(newWorldTiles, newWorldAssets, 20, 17);
    /* Set the static map pointer to current map */
  	World.SetMapAsCurrentView();
    /* Enter loading screen */
    Init.MainMenu();
    /* Create resource bar at the top of the screen */
    InfoDisplay TopDisplay(32, 5);
  	/* Add all the map tiles and assets to the render queue */
  	World.LoadMap();
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
