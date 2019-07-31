#include <memory>
#include "savemanager.h"
#include "interfaces.h"
#include "game.h"
#include "gui.h"
#include "text.h"
#include "image.h"
#include "user_interface.h"
#include "lpc_xav.h"
#include "geometry.h"

#define ROUGE 		0xE0
#define ORANGE 		0xF0
#define JAUNE		0xFC
#define VERT		0x1C
#define TURQUOISE 	0x1F
#define BLEU		0x03
#define BLEU_PALE 	0x32
#define VIOLET		0x83
#define ROSE		0xE3
#define BLANC		0xFF
#define NOIR		0x00
#define BACKGROUND	0x24

_I2C I2C_Bus((uint32_t *)0x4001C000, 14, 10);

const coords_pair_t UIPos = { .x = 280, .y = 180};
const uint8_t UIWidth = 12;
const uint8_t UIHeight = 10;
const coords_pair_t cancelButton = { .x = (int16_t)(UIPos.x + 15), .y = (int16_t)(UIPos.y + 190)};
const coords_pair_t saveButton = { .x = (int16_t)(UIPos.x + 165), .y = (int16_t)(UIPos.y + 190)};
const coords_pair_t returnButton = { .x = (int16_t)(UIPos.x + 15), .y = (int16_t)(UIPos.y + 143)};
const coords_pair_t selectButton = { .x = (int16_t)(UIPos.x + 145), .y = (int16_t)(UIPos.y + 143)};
const coords_pair_t entryBar = { .x = (int16_t)(UIPos.x + 60), .y = (int16_t)(UIPos.y + 35)};

void SaveManager::OnUserInteract()
{
	keyState.digitalState = 0;
	selection = 0;
	exit = false;
	validInput = false;
	cardDetected = true;

		for(uint8_t i = 0; i < 4; i++){
			uint16_t address = 0x2000 * i;
			I2C_Bus.New_Sequence(0xA0);
			I2C_Bus.Add_Byte(address >> 8);
			I2C_Bus.Add_Byte(0x00);
			I2C_Bus.Send_Sequence();

			while(I2C_Bus.Busy);

			I2C_Bus.New_Sequence(0xA1, 8);
			I2C_Bus.Send_Sequence();

			while(I2C_Bus.Busy);

			for(uint8_t j = 0; j < 8; j++){
				slotName[i][j] = I2C_Bus.rx_buf[j];
			}
			if(slotName[i][0] == 0xFF)
				strcpy(slotName[i], " ----  ");

			else if(strcmp(slotName[i], "\0\0\0\0\0\0\0\0") == 0)
				cardDetected = false;

			slotName[i][8] = '\0';
		}

		/* UI size = 280 x 240 */
		GUI = new UserInterface(UIPos.x, UIPos.y, UIWidth, UIHeight);

		GUI->AddElement(new Button(Button::RED, cancelButton, 4));
		GUI->AddElement(new Button(Button::GREEN, saveButton, 3));

		GUI->AddElement(new Image(ps2_circle, cancelButton.x + 90, cancelButton.y + 8, 24, 24, BLANC));
		GUI->AddElement(new Image(ps2_cross, saveButton.x + 70, saveButton.y + 8, 24, 24, BLANC));

	if(cardDetected){
		GUI->AddElement(new Text("Save the game", UIPos.x + 60, UIPos.y + 30, 16, BLANC));
		GUI->AddElement(new Text("Select memory slot", UIPos.x + 80, UIPos.y + 55, 10, BLANC));
		GUI->AddElement(new Text("Slot 1", UIPos.x + 60, UIPos.y + 100, 10, BLEU_PALE));
		GUI->AddElement(new Text(slotName[0], UIPos.x + 150, UIPos.y + 100, 10, BLEU_PALE));
		GUI->AddElement(new Text("Slot 2", UIPos.x + 60, UIPos.y + 120, 10, BLANC));
		GUI->AddElement(new Text(slotName[1], UIPos.x + 150, UIPos.y + 120, 10, BLANC));
		GUI->AddElement(new Text("Slot 3", UIPos.x + 60, UIPos.y + 140, 10, BLANC));
		GUI->AddElement(new Text(slotName[2], UIPos.x + 150, UIPos.y + 140, 10, BLANC));
		GUI->AddElement(new Text("Slot 4", UIPos.x + 60, UIPos.y + 160, 10, BLANC));
		GUI->AddElement(new Text(slotName[3], UIPos.x + 150, UIPos.y + 160, 10, BLANC));
		GUI->AddElement(new Text("Cancel", cancelButton.x + 10, cancelButton.y + 12, 16, BLANC));
		GUI->AddElement(new Text("Save", saveButton.x + 10, saveButton.y + 12, 16, BLANC));
	}
	else{
		GUI->AddElement(new Text("Error", UIPos.x + 110, UIPos.y + 40, 16, JAUNE));
		GUI->AddElement(new Text("Please insert memory card", UIPos.x + 60, UIPos.y + 100, 10, BLANC));
		GUI->AddElement(new Text("and try again", UIPos.x + 100, UIPos.y + 120, 10, BLANC));
		GUI->AddElement(new Text("Cancel", cancelButton.x + 10, cancelButton.y + 12, 16, BLANC));
		GUI->AddElement(new Text("Retry", saveButton.x + 10, saveButton.y + 12, 16, BLANC));
	}

	TickEngine::PushStack(this, Stack::L2);
}

void SaveManager::Tick()
{
	static uint8_t MenuState = 0;
	static uint8_t keyMap[3][10] = {
		{'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
		{'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',0xFE},
		{'z', 'x', 'c', 'v', 'b', 'n', 'm',0xFE,0xFE,0xFE}
	};
	const uint8_t lineOffset[3] = {20, 24, 34};
	static uint8_t entryLength;
	static char entryName[9];
	static char eraseBuf[9];
	static char tempBuf[27][2] = {'\0'};
	static char tempBuf2[27][2] = {'\0'};
	static int8_t rowSelect = 0, lineSelect = 0;

	PollController(0, &userInput);

	if(!userInput.Button.digitalState)
		validInput = true;

	if(cardDetected){
		if(validInput && GUI->isReady()){
			/* Slot select menu part */
			if(!MenuState){
				if(userInput.Button.X && keyState.X == RELEASED){
					MenuState++;

					for(uint8_t j = 0; j < 3; j++){
						for(uint8_t i = 0; i < 10; i++){
							if(keyMap[j][i] != 0xFE){
								keyMap[j][i] |= 0x20;
							}
						}
					}

					/* Erase previous menu text */
					GUI->AddElement(new Text("Save the game", UIPos.x + 60, UIPos.y + 30, 16, BACKGROUND));
					GUI->AddElement(new Text("Select memory slot", UIPos.x + 80, UIPos.y + 55, 10, BACKGROUND));
					GUI->AddElement(new Text(slotTitle[0], UIPos.x + 60, UIPos.y + 100, 10, BACKGROUND));
					GUI->AddElement(new Text(slotName[0], UIPos.x + 150, UIPos.y + 100, 10, BACKGROUND));
					GUI->AddElement(new Text(slotTitle[1], UIPos.x + 60, UIPos.y + 120, 10, BACKGROUND));
					GUI->AddElement(new Text(slotName[1], UIPos.x + 150, UIPos.y + 120, 10, BACKGROUND));
					GUI->AddElement(new Text(slotTitle[2], UIPos.x + 60, UIPos.y + 140, 10, BACKGROUND));
					GUI->AddElement(new Text(slotName[2], UIPos.x + 150, UIPos.y + 140, 10, BACKGROUND));
					GUI->AddElement(new Text(slotTitle[3], UIPos.x + 60, UIPos.y + 160, 10, BACKGROUND));
					GUI->AddElement(new Text(slotName[3], UIPos.x + 150, UIPos.y + 160, 10, BACKGROUND));

					GUI->AddElement(new Button(Button::BLUE, selectButton, 4));
					GUI->AddElement(new Button(Button::LIGHT_RED, returnButton, 4));
					GUI->AddElement(new Image(ps2_triangle, saveButton.x + 70, saveButton.y + 8, 24, 24, BLANC));
					GUI->AddElement(new Image(ps2_square, returnButton.x + 90, returnButton.y + 8, 24, 24, BLANC));
					GUI->AddElement(new Image(ps2_cross, selectButton.x + 90, selectButton.y + 8, 24, 24, BLANC));
					GUI->AddElement(new Rectangle(entryBar.x, entryBar.y, 160, 24, BLANC));
					GUI->AddElement(new Rectangle(UIPos.x + 202, UIPos.y + 113, 40, 20, BLANC));
					GUI->AddElement(new Text("Enter save name", UIPos.x + 55, UIPos.y + 13, 16, BLANC));
					GUI->AddElement(new Text("R1", UIPos.x + 208, UIPos.y + 117, 10, keyMap[0][0] == 'A'? BLEU_PALE : BLANC));
					GUI->AddElement(new Text("Select", selectButton.x + 10, selectButton.y + 12, 16, BLANC));
					GUI->AddElement(new Text("Return", returnButton.x + 10, returnButton.y + 12, 16, BLANC));

					/* Draws the keyboard rectangle map */
					for(uint8_t i = 0; i < 10; i++){
						GUI->AddElement(new Rectangle(UIPos.x + 20 + (i * 24), UIPos.y + 65, 20, 20, i == 0 ? BLEU_PALE : BLANC));
						tempBuf[i][0] = keyMap[0][i];
						GUI->AddElement(new Text(tempBuf[i], UIPos.x + 27 + (i * 24), UIPos.y + 69, 10, i == 0 ? BLEU_PALE : BLANC));
					}
					for(uint8_t i = 0; i < 9; i++){
						GUI->AddElement(new Rectangle(UIPos.x + 24 + (i * 24), UIPos.y + 89, 20, 20, BLANC));
						tempBuf[i + 10][0] = keyMap[1][i];
						GUI->AddElement(new Text(tempBuf[i + 10], UIPos.x + 31 + (i * 24), UIPos.y + 93, 10, BLANC));
					}
					for(uint8_t i = 0; i < 7; i++){
						GUI->AddElement(new Rectangle(UIPos.x + 34 + (i * 24), UIPos.y + 113, 20, 20, BLANC));
						tempBuf[i + 19][0] = keyMap[2][i];
						GUI->AddElement(new Text(tempBuf[i + 19], UIPos.x + 41 + (i * 24), UIPos.y + 117, 10, BLANC));
					}

					entryLength = 0;
					if(slotName[selection][1] != '-'){
						strcpy(entryName, slotName[selection]);
						for(uint8_t i = 0; entryName[i] != '\0'; i++, entryLength++);
					}
					else
						memset(entryName, 0, sizeof(entryName));

					/* Display currently composed save name */
					GUI->AddElement(new Text(entryName, entryBar.x + 10, entryBar.y + 7, 10, BLANC));
				}

				if(userInput.Button.Up && keyState.Up == RELEASED){
					GUI->AddElement(new Text(slotTitle[selection], UIPos.x + 60, UIPos.y + 100 + (selection * 20), 10, BLANC));
					GUI->AddElement(new Text(slotName[selection], UIPos.x + 150, UIPos.y + 100 + (selection * 20), 10, BLANC));

					if(selection > 0)
						selection--;
					else
						selection = 3;

					GUI->AddElement(new Text(slotTitle[selection], UIPos.x + 60, UIPos.y + 100 + (selection * 20), 10, BLEU_PALE));
					GUI->AddElement(new Text(slotName[selection], UIPos.x + 150, UIPos.y + 100 + (selection * 20), 10, BLEU_PALE));
				}

				if(userInput.Button.Down && keyState.Down == RELEASED){
					GUI->AddElement(new Text(slotTitle[selection], UIPos.x + 60, UIPos.y + 100 + (selection * 20), 10, BLANC));
					GUI->AddElement(new Text(slotName[selection], UIPos.x + 150, UIPos.y + 100 + (selection * 20), 10, BLANC));

					selection++;
					selection %= 4;

					GUI->AddElement(new Text(slotTitle[selection], UIPos.x + 60, UIPos.y + 100 + (selection * 20), 10, BLEU_PALE));
					GUI->AddElement(new Text(slotName[selection], UIPos.x + 150, UIPos.y + 100 + (selection * 20), 10, BLEU_PALE));
				}

				if(userInput.Button.O && keyState.O == RELEASED){
					exit = true;
				}
			}
			/***************************************************************************/
			/* Name entry menu part */
			else{
				if(userInput.Button.Triangle && keyState.Triangle == RELEASED){
					uint8_t * dataPtr = (uint8_t *)&memoryBuffer;
					memoryBuffer.Resources = Game::Resources;
					memoryBuffer.Prod = Game::ProdPer5;
					strcpy(memoryBuffer.saveName, entryName);
					for(size_t i = 0; i < sizeof(memoryBuffer);){
						I2C_Bus.New_Sequence(0xA0);
						I2C_Bus.Add_Byte((selection * 0x20) + (i >> 8));
						I2C_Bus.Add_Byte(i);

						for(uint8_t j = 0; j < 64; j++, ++i){
							I2C_Bus.Add_Byte(*dataPtr++);
							if(i == sizeof(memoryBuffer))
								break;
						}

						I2C_Bus.Send_Sequence();
						while(I2C_Bus.Busy);

						if(i == sizeof(memoryBuffer))
							break;
						else
							for(uint32_t delay = (LPC_TIMER0->TC + 960000); LPC_TIMER0->TC < delay;);


					}
					exit = true;
				}

				if(userInput.Button.Up && keyState.Up == RELEASED){
					if(lineSelect > 0){
						GUI->AddElement(new Rectangle(UIPos.x + lineOffset[lineSelect] + (rowSelect * 24), UIPos.y + 65 + (lineSelect * 24), 20, 20, BLANC));
						tempBuf[(lineSelect * 10) + rowSelect][0] = keyMap[lineSelect][rowSelect];
						GUI->AddElement(new Text(tempBuf[(lineSelect * 10) + rowSelect], UIPos.x + lineOffset[lineSelect] + 7 + (rowSelect * 24), UIPos.y + 69 + (lineSelect * 24), 10, BLANC));
						lineSelect--;
						GUI->AddElement(new Rectangle(UIPos.x + lineOffset[lineSelect] + (rowSelect * 24), UIPos.y + 65 + (lineSelect * 24), 20, 20, BLEU_PALE));
						tempBuf[(lineSelect * 10) + rowSelect][0] = keyMap[lineSelect][rowSelect];
						GUI->AddElement(new Text(tempBuf[(lineSelect * 10) + rowSelect], UIPos.x + lineOffset[lineSelect] + 7 + (rowSelect * 24), UIPos.y + 69 + (lineSelect * 24), 10, BLEU_PALE));
					}
				}

				if(userInput.Button.Down && keyState.Down == RELEASED){
					if(lineSelect < 2 && (keyMap[lineSelect + 1][rowSelect] != 0xFE)){
						GUI->AddElement(new Rectangle(UIPos.x + lineOffset[lineSelect] + (rowSelect * 24), UIPos.y + 65 + (lineSelect * 24), 20, 20, BLANC));
						tempBuf[(lineSelect * 10) + rowSelect][0] = keyMap[lineSelect][rowSelect];
						GUI->AddElement(new Text(tempBuf[(lineSelect * 10) + rowSelect], UIPos.x + lineOffset[lineSelect] + 7 + (rowSelect * 24), UIPos.y + 69 + (lineSelect * 24), 10, BLANC));
						lineSelect++;
						GUI->AddElement(new Rectangle(UIPos.x + lineOffset[lineSelect] + (rowSelect * 24), UIPos.y + 65 + (lineSelect * 24), 20, 20, BLEU_PALE));
						tempBuf[(lineSelect * 10) + rowSelect][0] = keyMap[lineSelect][rowSelect];
						GUI->AddElement(new Text(tempBuf[(lineSelect * 10) + rowSelect], UIPos.x + lineOffset[lineSelect] + 7 + (rowSelect * 24), UIPos.y + 69 + (lineSelect * 24), 10, BLEU_PALE));
					}
				}

				if(userInput.Button.Left && keyState.Left == RELEASED){
					if(rowSelect > 0){
						GUI->AddElement(new Rectangle(UIPos.x + lineOffset[lineSelect] + (rowSelect * 24), UIPos.y + 65 + (lineSelect * 24), 20, 20, BLANC));
						tempBuf[(lineSelect * 10) + rowSelect][0] = keyMap[lineSelect][rowSelect];
						GUI->AddElement(new Text(tempBuf[(lineSelect * 10) + rowSelect], UIPos.x + lineOffset[lineSelect] + 7 + (rowSelect * 24), UIPos.y + 69 + (lineSelect * 24), 10, BLANC));
						rowSelect--;
						GUI->AddElement(new Rectangle(UIPos.x + lineOffset[lineSelect] + (rowSelect * 24), UIPos.y + 65 + (lineSelect * 24), 20, 20, BLEU_PALE));
						tempBuf[(lineSelect * 10) + rowSelect][0] = keyMap[lineSelect][rowSelect];
						GUI->AddElement(new Text(tempBuf[(lineSelect * 10) + rowSelect], UIPos.x + lineOffset[lineSelect] + 7 + (rowSelect * 24), UIPos.y + 69 + (lineSelect * 24), 10, BLEU_PALE));
					}
				}

				if(userInput.Button.Right && keyState.Right == RELEASED){
					if(rowSelect < 9 && (keyMap[lineSelect][rowSelect + 1] != 0xFE)){
						GUI->AddElement(new Rectangle(UIPos.x + lineOffset[lineSelect] + (rowSelect * 24), UIPos.y + 65 + (lineSelect * 24), 20, 20, BLANC));
						tempBuf[(lineSelect * 10) + rowSelect][0] = keyMap[lineSelect][rowSelect];
						GUI->AddElement(new Text(tempBuf[(lineSelect * 10) + rowSelect], UIPos.x + lineOffset[lineSelect] + 7 + (rowSelect * 24), UIPos.y + 69 + (lineSelect * 24), 10, BLANC));
						rowSelect++;
						GUI->AddElement(new Rectangle(UIPos.x + lineOffset[lineSelect] + (rowSelect * 24), UIPos.y + 65 + (lineSelect * 24), 20, 20, BLEU_PALE));
						tempBuf[(lineSelect * 10) + rowSelect][0] = keyMap[lineSelect][rowSelect];
						GUI->AddElement(new Text(tempBuf[(lineSelect * 10) + rowSelect], UIPos.x + lineOffset[lineSelect] + 7 + (rowSelect * 24), UIPos.y + 69 + (lineSelect * 24), 10, BLEU_PALE));
					}
				}

				if(userInput.Button.R1 && keyState.R1 == RELEASED){
					/* erase keyboard characters */
					for(uint8_t i = 0; i < 10; i++){
						tempBuf[i][0] = keyMap[0][i];
						GUI->AddElement(new Text(tempBuf[i], UIPos.x + 27 + (i * 24), UIPos.y + 69, 10, BACKGROUND));
					}
					for(uint8_t i = 0; i < 9; i++){
						tempBuf[i + 10][0] = keyMap[1][i];
						GUI->AddElement(new Text(tempBuf[i + 10], UIPos.x + 31 + (i * 24), UIPos.y + 93, 10, BACKGROUND));
					}
					for(uint8_t i = 0; i < 7; i++){
						tempBuf[i + 19][0] = keyMap[2][i];
						GUI->AddElement(new Text(tempBuf[i + 19], UIPos.x + 41 + (i * 24), UIPos.y + 117, 10, BACKGROUND));
					}

					for(uint8_t j = 0; j < 3; j++){
						for(uint8_t i = 0; i < 10; i++){
							if(keyMap[j][i] != 0xFE){
								keyMap[j][i] ^= 0x20;
							}
						}
					}

					/* draw new character set */
					for(uint8_t i = 0; i < 10; i++){
						tempBuf2[i][0] = keyMap[0][i];
						GUI->AddElement(new Text(tempBuf2[i], UIPos.x + 27 + (i * 24), UIPos.y + 69, 10, (lineSelect == 0 && rowSelect == i)? BLEU_PALE : BLANC));
					}
					for(uint8_t i = 0; i < 9; i++){
						tempBuf2[i + 10][0] = keyMap[1][i];
						GUI->AddElement(new Text(tempBuf2[i + 10], UIPos.x + 31 + (i * 24), UIPos.y + 93, 10, (lineSelect == 1 && rowSelect == i)? BLEU_PALE : BLANC));
					}
					for(uint8_t i = 0; i < 7; i++){
						tempBuf2[i + 19][0] = keyMap[2][i];
						GUI->AddElement(new Text(tempBuf2[i + 19], UIPos.x + 41 + (i * 24), UIPos.y + 117, 10, (lineSelect == 2 && rowSelect == i)? BLEU_PALE : BLANC));
					}

					GUI->AddElement(new Rectangle(UIPos.x + 202, UIPos.y + 113, 40, 20, keyMap[0][0] == 'Q'? BLEU_PALE : BLANC));
					GUI->AddElement(new Text("R1", UIPos.x + 208, UIPos.y + 117, 10, keyMap[0][0] == 'Q'? BLEU_PALE : BLANC));
				}

				if(userInput.Button.X && keyState.X == RELEASED){
					if(entryLength < 8){
						entryName[entryLength++] = keyMap[lineSelect][rowSelect];
						GUI->AddElement(new Text(entryName, entryBar.x + 10, entryBar.y + 7, 10, BLANC));
					}
				}

				if(userInput.Button.Square && keyState.Square == RELEASED){
					if(entryLength > 0){
						strcpy(eraseBuf, entryName);
						entryName[entryLength - 1] = '\0';
						entryLength--;
						GUI->AddElement(new Text(eraseBuf, entryBar.x + 10, entryBar.y + 7, 10, BACKGROUND));
						GUI->AddElement(new Text(entryName, entryBar.x + 10, entryBar.y + 7, 10, BLANC));
					}
				}

				if(userInput.Button.O && keyState.O == RELEASED){
					exit = true;
				}
			}
		}
	}
	/* Goes here if no card present in memory card slot */
	else{
		if(userInput.Button.X && keyState.X == RELEASED){
			TickEngine::PopStack(Stack::L2);
			this->OnUserInteract();
		}

		if(userInput.Button.O && keyState.O == RELEASED){
			exit = true;
		}
	}

	keyState.digitalState |= userInput.Button.digitalState;
	keyState.digitalState &= userInput.Button.digitalState;

	/* Closing sequence */
	if(!GUI->ProcessGraphics(exit)){
		MenuState = 0;
		rowSelect = 0;
		lineSelect = 0;
		memset(entryName, 0, sizeof(entryName));
		delete GUI;
		TickEngine::PopStack(Stack::L2);
	}
}


