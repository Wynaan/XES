#include "lpc_xav.h"
#include "savemanager.h"

extern _I2C I2C_Bus;


extern "C" void I2C0_IRQHandler(void)
{
	uint8_t Status = I2C_Bus.get_Status();

	switch(Status){
	case 0x08: // STA transmitted
		I2C_Bus.Send_Address(); // SLA+W/R will be transmitted
		break;
	case 0x18: // SLA+W transmitted, ACK received
		I2C_Bus.Send_Next_Byte(); // Begin sending data
		break;
	case 0x28: // Byte in I2DAT transmitted, ACK received
		if(I2C_Bus.M_Counter_tx == I2C_Bus.Msg_Length){
			I2C_Bus.Send_Stop(); // Send stop if last byte
		}
		else{
			I2C_Bus.Send_Next_Byte(); // Send next data
		}
		break;
	case 0x20: // NOT ACK received
		I2C_Bus.Send_Stop();
		break;
	case 0x30: // NOT ACK received
		I2C_Bus.Send_Stop();
		break;
	case 0x40: // SLA+R transmitted, ACK received
		I2C_Bus.Clear_Interrupt(); // Send next data
		break;
	case 0x48: // NOT ACK received
		I2C_Bus.Send_Stop();
		break;
	case 0x50: // Data received, ACK received
		if(I2C_Bus.M_Counter_rx == I2C_Bus.Read_Length){
			I2C_Bus.Send_Stop(); // Send stop if last byte
		}
		else{
			I2C_Bus.Read_Next_Byte(); // Send next data
		}
		break;
	case 0x58: // Data received, NOT ACK received
		I2C_Bus.Read_Next_Byte();
		I2C_Bus.Send_Stop();
		break;
	case 0xF8:
		break;
	}
}


