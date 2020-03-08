#include "ps2controller.h"
#include "lpc_xav.h"
#include <stdint.h>
#include "board.h"

extern LPC_IO Controller1Select;
extern LPC_IO Controller2Select;

/* All command bytes are reversed to account for LSB-first */
uint8_t enterConfig[5] = {0x80, 0xC2, 0x00, 0x80, 0x00};
uint8_t setAnalogMode[9] = {0x80, 0x22, 0x00, 0x80, 0xC0, 0x00, 0x00, 0x00, 0x00};
uint8_t enableRumble[9] = {0x80, 0xB2, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t exitConfig[9] = {0x80, 0xC2, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};
uint8_t poll[5] = {0x80, 0x42, 0x00, 0x00, 0x00};

uint8_t reverseBits(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

bool InitPS2Controller(bool controllerId)
{
	uint8_t bufferPoll[10];

	if(!(LPC_SSP1->SR & 0x10)){
		Controller1Select = 0;

		/* Empty SSP1 receive buffer */
		while(LPC_SSP1->SR & 0x04){
			LPC_SSP1->DR;
		}

		/* Do a dummy polling */
		for(uint8_t i = 0; i < 5; i++){
			/* Send each command byte */
			LPC_SSP1->DR = poll[i];
			while(LPC_SSP1->SR & 0x10);

			/* Wait for controller acknowledge */
			for(int i = 0; i < 100; i++);
		}

		/* Get response from controller */
		for(int i = 0;LPC_SSP1->SR & 0x04; i++){
			bufferPoll[i] = LPC_SSP1->DR;
		}

		/* Toggle attention line */
		Controller1Select = 1;
		for(int i = 0; i < 200; i++);
		Controller1Select = 0;

		/* Enter configuration mode */
		for(uint8_t i = 0; i < 5; i++){
			/* Send each command byte */
			LPC_SSP1->DR = enterConfig[i];
			while(LPC_SSP1->SR & 0x10);

			/* Wait for controller acknowledge */
			for(int i = 0; i < 100; i++);

			/* Ignore response from controller */
			if(LPC_SSP1->SR & 0x04)
				LPC_SSP1->DR;
		}

		/* Toggle attention line */
		Controller1Select = 1;
		for(int i = 0; i < 200; i++);
		Controller1Select = 0;

		/* Enable analog mode if not already enabled*/
		if((bufferPoll[1] & 0x0F) == 0x2){
			for(uint8_t i = 0; i < 9; i++){
				/* Send each command byte */
				LPC_SSP1->DR = setAnalogMode[i];
				while(LPC_SSP1->SR & 0x10);

				/* Wait for controller acknowledge */
				for(int i = 0; i < 100; i++);

				/* Ignore response from controller */
				if(LPC_SSP1->SR & 0x04)
					LPC_SSP1->DR;
			}

			/* Toggle attention line */
			Controller1Select = 1;
			for(int i = 0; i < 200; i++);
			Controller1Select = 0;
		}

		/* Enable rumble motors */
		for(uint8_t i = 0; i < 9; i++){
			/* Send each command byte */
			LPC_SSP1->DR = enableRumble[i];
			while(LPC_SSP1->SR & 0x10);

			/* Wait for controller acknowledge */
			for(int i = 0; i < 100; i++);

			/* Ignore response from controller */
			if(LPC_SSP1->SR & 0x04)
				LPC_SSP1->DR;
		}

		/* Toggle attention line */
		Controller1Select = 1;
		for(int i = 0; i < 200; i++);
		Controller1Select = 0;

		/* Exit configuration mode */
		for(uint8_t i = 0; i < 9; i++){
			/* Send each command byte */
			LPC_SSP1->DR = exitConfig[i];
			while(LPC_SSP1->SR & 0x10);

			/* Wait for controller acknowledge */
			for(int i = 0; i < 100; i++);

			/* Ignore response from controller */
			if(LPC_SSP1->SR & 0x04)
				LPC_SSP1->DR;
		}

		Controller1Select = 1;

		return true;
	}
	else
		return false;
}

bool PollController(bool controllerId, ps2_inputs_t * result)
{
	/* Commands bit order must be reversed */
	if(!(LPC_SSP1->SR & 0x10)){
		/* Clear all values in structure pointer */
		result->Button.digitalState = 0xFFFF;

		/* Empty SSP1 receive buffer */
		while(LPC_SSP1->SR & 0x04){
			LPC_SSP1->DR;
		}

		/* Drive attention line low */
		Controller1Select = 0;

		/* Send initial header byte, always 0x01 */
		LPC_SSP1->DR = 0x80;
		while(LPC_SSP1->SR & 0x10);

		/* Wait for controller acknowledge */
		for(int i = 0; i < 100; i++);

		/* Ignore header replies from controller */
		LPC_SSP1->DR;

		/* Send main polling command */
		LPC_SSP1->DR = 0x42;
		while(LPC_SSP1->SR & 0x10);

		/* Wait for controller acknowledge */
		for(int i = 0; i < 100; i++);

		/* Send third header byte, always 0x00 */
		LPC_SSP1->DR = 0x00;
		while(LPC_SSP1->SR & 0x10);

		/* Wait for controller acknowledge */
		for(int i = 0; i < 100; i++);

		/* Send 1st command byte */
		LPC_SSP1->DR = 0x00; // TODO: change for activating rumble
		while(LPC_SSP1->SR & 0x10);

		/* Wait for controller acknowledge */
		for(int i = 0; i < 100; i++);

		/* Send 2nd command byte */
		LPC_SSP1->DR = 0x00;
		while(LPC_SSP1->SR & 0x10);

		/* Wait for controller acknowledge */
		for(int i = 0; i < 100; i++);

		/* Send 3rd command byte */
		LPC_SSP1->DR = 0x00;
		while(LPC_SSP1->SR & 0x10);

		/* Wait for controller acknowledge */
		for(int i = 0; i < 100; i++);

		/* Send 4th command byte */
		LPC_SSP1->DR = 0x00;
		while(LPC_SSP1->SR & 0x10);

		/* Wait for controller acknowledge */
		for(int i = 0; i < 100; i++);

		/* Send 5th command byte */
		LPC_SSP1->DR = 0x00;
		while(LPC_SSP1->SR & 0x10);

		/* Wait for controller acknowledge */
		for(int i = 0; i < 100; i++);

		/* Send 6th command byte */
		LPC_SSP1->DR = 0x00;
		while(LPC_SSP1->SR & 0x10);

		/* Ignore header replies from controller */
		LPC_SSP1->DR;
		LPC_SSP1->DR;
		/* Get first 8 digital buttons status */
		result->Button.digitalState ^= LPC_SSP1->DR << 8;
		/* Get last 8 digital buttons status */
		result->Button.digitalState ^= LPC_SSP1->DR;
		/* Get right joystick X axis value*/
		result->JoystickRightX = reverseBits((uint8_t)LPC_SSP1->DR);
		/* Get right joystick Y axis value*/
		result->JoystickRightY = reverseBits((uint8_t)LPC_SSP1->DR);
		/* Get left joystick X axis value*/
		result->JoystickLeftX = reverseBits((uint8_t)LPC_SSP1->DR);
		/* Get left joystick Y axis value*/
		result->JoystickLeftY = reverseBits((uint8_t)LPC_SSP1->DR);

		/* Drive attention line high to signal end of polling */
		Controller1Select = 1;

		return true;
	}
	else
		return false;
}
