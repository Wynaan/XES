#ifndef PS2CONTROLLER_H_
#define PS2CONTROLLER_H_
#include <stdint.h>

typedef union{
	uint16_t digitalState;
	struct{
		/* Bit order reversed */
		unsigned Square		:1;
		unsigned X			:1;
		unsigned O			:1;
		unsigned Triangle	:1;
		unsigned R1			:1;
		unsigned L1			:1;
		unsigned R2			:1;
		unsigned L2			:1;
		/* Bit order reversed */
		unsigned Left		:1;
		unsigned Down		:1;
		unsigned Right		:1;
		unsigned Up			:1;
		unsigned Start		:1;
		unsigned R3			:1;
		unsigned L3			:1;
		unsigned Select 	:1;
	};
}ps2_buttons_t;

typedef struct{
	ps2_buttons_t Button;
	uint8_t JoystickRightX;
	uint8_t JoystickRightY;
	uint8_t JoystickLeftX;
	uint8_t JoystickLeftY;
}ps2_inputs_t;

bool InitPS2Controller(bool controllerId);

bool PollController(bool controllerId, ps2_inputs_t * result);


#endif /* PS2CONTROLLER_H_ */
