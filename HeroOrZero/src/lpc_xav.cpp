#include "lpc_xav.h"
#include "segdisplay.h"

extern bool Transfer_Done;

void EnableIRQ(uint8_t Int_bit)
{
	*((uint32_t *) 0xE000E280) |= (1 << Int_bit); // Clear pending interrupt
	*((uint32_t *) 0xE000E100) |= (1 << Int_bit); // Enable interrupt
}

void GPIO_Enable_Int(uint8_t PortNo, uint8_t PinNo, uint8_t Edge_Select)
{
	if(PortNo == 0)
		*((uint32_t *)0x40028090 + Edge_Select - 1) |= (1 << PinNo);
	if(PortNo == 2)
		*((uint32_t *)0x400280B4 + Edge_Select - 1) |= (1 << PinNo);

	EnableIRQ(21);
}

void Enable_EXT_Int(uint8_t ExtIntNo, uint8_t ExtMode) // Enable EXT[0..3] interrupts
{
	EnableIRQ(ExtIntNo + 18);

	*(uint32_t*)0x400FC148 &= ~(1 << ExtIntNo);
	*(uint32_t*)0x400FC148 |= (ExtMode << ExtIntNo);
}
/***********************************************************************************************************/
void Port::SetFct(uint8_t PinNo, uint8_t Fct) // Set function for pin (GPIO, periph, etc)
{
	if(PinNo > 15){
		*(_PINCB + 1) &= ~(3 << ((PinNo-16)*2));
		*(_PINCB + 1) |= (Fct << ((PinNo-16)*2));
	}
	else{
		*_PINCB &= ~(3 << (PinNo*2));
		*_PINCB |= (Fct << (PinNo*2));
	}
}

void Port::SetDir(uint8_t PinNo, uint8_t Dir) // Set pin in input/ouput
{
	*_FIO &= ~(1 << PinNo);
	*_FIO |= (Dir << PinNo);
}

void Port::SetMode(uint8_t PinNo, uint8_t Mode) // Set mode (float, p-up, p-down)
{
	if(PinNo > 15){
		*(_PINCB + 17) &= ~(3 << ((PinNo-16)*2));
		*(_PINCB + 17) |= (Mode << ((PinNo-16)*2));
	}
	else{
	*(_PINCB + 16) &= ~(3 << PinNo*2);
	*(_PINCB + 16) |= (Mode << (PinNo*2));
	}
}

void Port::SetOpenDrain(uint8_t PinNo, uint8_t Mode) // Enable open drain for pin
{
	*_OD &= ~(1 << PinNo);
	if(Mode == Open_Drain)
		*_OD |= (Mode << PinNo);
}

Port::Port(	uint32_t* Pinsel_Add, // Constructor for LPC Port handles
		 	uint32_t* FastIO_Add,
			uint32_t* OpenDrain_Add)
		 :_PINCB(Pinsel_Add),
		 _FIO(FastIO_Add),
		 _OD(OpenDrain_Add)
	{}
/***********************************************************************************************************/
void LPC_IO::Out(bool State)
{
	if (State == 0)
		*(_Port._FIO + 5) &= ~(1 << Pin);
	else
		*(_Port._FIO + 5) |= (1 << Pin);
}

void LPC_IO::Set()
{
	*(_Port._FIO + 6) |= (1 << Pin);
}

void LPC_IO::Clear()
{
	*(_Port._FIO + 7) |= (1 << Pin);
}

LPC_IO::LPC_IO(	Port& RefPort,
				uint8_t PinNo,
				uint8_t Fct,
				uint8_t Dir,
				uint8_t Md,
				uint8_t OD)
	:_Port(RefPort),
	 Pin(PinNo),
	 Function(Fct),
	 Direction(Dir),
	 Mode(Md),
	 OD_Mode(OD)
{
	_Port.SetFct(Pin, Function);

	if(Function == GPIO)
		_Port.SetDir(Pin, Direction);

	if(Direction == IN)
		_Port.SetMode(Pin, Mode);

	_Port.SetOpenDrain(Pin, OD);

}
/***********************************************************************************************************/
void Bus::Change_Value(uint8_t bus_value)
{
	Value = bus_value;
	for(int i = 0; Bits[i] != nullptr && i < 8; i++)
		Bits[i]->Out((bus_value >> i) & 0x1);
}

Bus::Bus(LPC_IO* LSB, LPC_IO* B1, LPC_IO* B2, LPC_IO* B3,
		LPC_IO* B4, LPC_IO* B5, LPC_IO* B6, LPC_IO* MSB)
{
	Bits[0] = LSB;
	Bits[1] = B1;
	Bits[2] = B2;
	Bits[3] = B3;
	Bits[4] = B4;
	Bits[5] = B5;
	Bits[6] = B6;
	Bits[7] = MSB;

	Value = 0;
}
/***********************************************************************************************************/
void Timer::Init(uint8_t Clk_Speed)
{
	PCONP |= (1 << Pwr_bit);

	*(_Periph + 1) = 1; // Set counter enable bit in TCR

	if(IRQ_bit < 3){ // Set clock speed for corresponding timer
		PCLKSEL0 &= ~(3 << Clk_bit);
		PCLKSEL0 |= (Clk_Speed << Clk_bit);
	}
	else{
		PCLKSEL1 &= ~(3 << Clk_bit);
		PCLKSEL1 |= (Clk_Speed << Clk_bit);
	}
}

void Timer::Enable_Counter(uint8_t Channel, uint8_t Edge_Mode)
{
	*(_Periph + 14) = (Edge_Mode + 1); // Enable counter mode (incremented on CAP pin
	*(_Periph + 14) |= (Channel << 2); // edges)
}

void Timer::Enable_Capture(uint8_t Channel, uint8_t Edge_Mode)
{
	*(_Periph + 10) &= ~(0x3 << (Channel * 3)); // Set CAPxRE or CAPxFE in CCR
	*(_Periph + 10) |= (Edge_Mode << (Channel * 3)); // for corresponding channel

	Edge_Sel[Channel] = Edge_Mode;
}

void Timer::Reset_onMatch(uint8_t Channel)
{
	*(_Periph + 5) |= (0x2 << (Channel * 3)); // Set MRxR in MCR for selected channel
}

void Timer::Set_Match(uint8_t Channel, uint32_t Match_Value)
{
	*(_Periph + 6 + Channel) = Match_Value; // Set MRx value for selected channel
}

void Timer::Enable_Ext_Match(uint8_t Channel, uint8_t Match_Setting)
{
	*(_Periph + 13) &= ~(3 << (4 + Channel * 2));
	*(_Periph + 13) |= (Match_Setting << (4 + Channel * 2));
}

void Timer::Enable_Mat_Int(uint8_t Channel)
{
	*(_Periph + 5) |= (0x1 << (Channel * 3)); // Set MRxI in MCR for selected channel
	EnableIRQ(IRQ_bit);
}

void Timer::Enable_Cap_Int(uint8_t Channel)
{
	*(_Periph + 10) |= (0x4 << (Channel * 3)); // Set CAPxI in CCR for selected channel
	EnableIRQ(IRQ_bit);
}

void Timer::Reset()
{
	*(_Periph + 1) = 2; // Set Reset bit in TCR
	*(_Periph + 1) = 1; // Clear reset and set count enable bit in TCR
}

uint8_t Timer::get_IR()
{
	return *_Periph; // Return TxIR value
}

void Timer::Clear_Int(uint8_t Flag)
{
	*_Periph |= Flag; // Clear selected flag in TxIR
}

uint32_t Timer::get_CR(uint8_t Channel)
{
	return (*(_Periph + 11 + Channel)); // Return TxCRx for selected channel
}

Timer::Timer(uint32_t * PtrTimer, uint8_t P_bit, uint8_t C_bit, uint8_t Int_bit)
	:_Periph(PtrTimer),
	Pwr_bit(P_bit),
	Clk_bit(C_bit),
	IRQ_bit(Int_bit)
	{}
/***********************************************************************************************************/
void UART::Init(uint32_t Baudrate)
{
	PCONP |= (1 << Pwr_bit);

	if(IRQ_bit < 7){ // Set CCLK/1 speed for corresponding UART module
		PCLKSEL0 &= ~(3 << Clk_bit);
		PCLKSEL0 |= (CLK0 << Clk_bit);
	}
	else{
		PCLKSEL1 &= ~(3 << Clk_bit);
		PCLKSEL1 |= (CLK0 << Clk_bit);
	}

	*(_Periph + 3) |= 0x80; // Set DLAB

	uint32_t Divisor = SYS_FREQ/(Baudrate << 4);
	*_Periph = Divisor % 256; // Set DLL in UxDLL
	*(_Periph + 1) = Divisor / 256; // Set DLM in UxDLM

	*(_Periph + 3) &= 0x7F; // Clear DLAB

	*(_Periph + 2) |= 0x01; // Enable FIFO
}

void UART::Set_Transmission(uint8_t Bit_Length, uint8_t Parity)
{
	*(_Periph + 3) &= 0xE0; // Clear settings in UxLCR

	*(_Periph + 3) |= Bit_Length - 5; // Set bits [0..1] in UxLCR

	*(_Periph + 3) &= ~(0x7 << 3); // Clear parity enable and select in UxLCR
	*(_Periph + 3) |= (Parity << 3); // Set parity select bits in UxLCR
}

void UART::PutByte(char Byte)
{
	*_Periph = Byte; // Put data byte in UxTHR

	while(!(*(_Periph + 5) & 0x20)); // Read UxLSR until THR bit is set (empty TX register)
}

void UART::PrintStrConst(const char *StrToSend)
{
	for(; *StrToSend; StrToSend++){
	    PutByte(*StrToSend);
	}
}


void UART::PrintDec(uint16_t NbrDec)
{
	IntegerToBCD(Console_BCD, NbrDec);

	if(NbrDec > 999)
		PutByte(Console_BCD.mil + 48);
	if(NbrDec > 99)
		PutByte(Console_BCD.cen + 48);
	if(NbrDec > 9)
		PutByte(Console_BCD.diz + 48);
    PutByte(Console_BCD.uni + 48);
}

void UART::PrintFloat(uint16_t Scaled_Int) // Used to print float number from an integer
{
	IntegerToBCD(Console_BCD, Scaled_Int);
    if (Scaled_Int < 10000)
        PutByte(Console_BCD.mil + 48);
        // Handle case where desired number is 100.0%
    else {
        PutByte('1');
        PutByte('0');
    }
    PutByte(Console_BCD.cen + 48);
    PutByte('.');
    PutByte(Console_BCD.diz + 48);
    PutByte(Console_BCD.uni + 48);
}

void UART::ClearScreen()
{
	for(int i = 0; i < 35; i++){
		PutByte('\r');
		PutByte('\n');
	}
}

void UART::Enable_RX_Int()
{
	*(_Periph + 1) = 0x01; // Set RBR Int Enable for interrupts upon reception
	EnableIRQ(IRQ_bit);
}

bool UART::Int_Flag_Set()
{
	if(*(_Periph + 2) & 0x000F){ // Read UxIIR for int status bits
		if(*(_Periph + 5) == 0x61) // Read UxLSR for UART status
			return true;
		else
			return false;
	}
	else
		return false;
}

uint8_t UART::get_RBR()
{
	return *_Periph; // Return UxRBR register contents
}

UART::UART(uint32_t * PtrUART, uint8_t P_bit, uint8_t C_bit, uint8_t Int_bit)
	:_Periph(PtrUART),
	Pwr_bit(P_bit),
	Clk_bit(C_bit),
	IRQ_bit(Int_bit)
{}
/***********************************************************************************************************/
uint32_t * PWM::_Periph = (uint32_t *)0x40018000; // Definition of the PWM static address

void PWM::Set_Freq(uint32_t Freq)
{
	*(_Periph + 5) |= 0x2; // Set PWMMR0R bit in MCR for reset upon TC = MR0
	*(_Periph + 6) = (SYS_FREQ >> 2) / Freq; // Sets MR0

	*(_Periph + 20) |= 0x1; // Set LER bit for MR0 update
}

void PWM::Reset_onMatch()
{
	*(_Periph + 5) |= (0x10 << Output_Channel * 3); // Set PWMMRxI for selected channel
}

void PWM::Set_Duty(float Duty_Percent)
{
	Duty = Duty_Percent;
	// Set MRx of corresponding channel at selected fraction of MR0 value
	*(_Periph + 6 + Output_Channel) = (*(_Periph + 6) / 100 * Duty);

	// Set LER bit for corresponding channel to update MRx value
	*(_Periph + 20) |= (1 << Output_Channel);
}

void PWM::Increment_Duty()
{
	if(Duty < 100)
		Set_Duty(Duty + 1);
}

void PWM::Decrement_Duty()
{
	if(Duty > 5)
		Set_Duty(Duty - 1);
}

void PWM::Enable()
{
	// Set PWM Enable and count enable bit in PWM1TCR,
	// must be done at the end of initialization
	*(_Periph + 1) |= 0x09;
}

PWM::PWM(uint8_t Ch, uint8_t Edge_Sel)
{
	PCONP |= (1 << 6); // Set PCONP bit for PWM

	Output_Channel = Ch;

	*(_Periph + 19) &= ~(1 << Output_Channel); // Clear previous settings

	*(_Periph + 19) |= (Edge_Sel << Output_Channel); // Select single-edge or double-edge in PCR

	*(_Periph + 19) |= (1 << (Output_Channel + 8)); // Enable PWM output of selected channel in PCR
}
/***********************************************************************************************************/
uint32_t * ADC::_Periph = (uint32_t *)0x40034000; // Definition of the ADC static address

void ADC::Start_Conversion()
{
	*_Periph |= (1 << Output_Channel); // Set Select bit for this ADC channel

	*_Periph |= (1 << 24); // Set Start now bit
}

void ADC::Start_Burst_Conversion(uint8_t Sel_Val)
{
	*_Periph |= Sel_Val + (1 << 16); // Set Burst bit and SEL bits
}

void ADC::Stop_Conversion()
{
	*_Periph &= ~(1 << Output_Channel); // Clear Select bit for this ADC channel

	*_Periph &= ~(1 << 24); // Clear Start Now bit
}

void ADC::Stop_Burst_Conversion()
{
	*_Periph &= ~(0xFF); // Clear all Select bits

	*_Periph &= ~(1 << 16); // Clear Burst bit
}

void ADC::Enable_Int()
{
	*(_Periph + 3) |= (1 << Output_Channel); // Set ADINTEN for specified channel

	*(_Periph + 3) &= ~(1 << 8); // Clear ADGINTEN

	EnableIRQ(22);
}

uint16_t ADC::get_12bit_Result()
{
	// Return value in [4..15] of corresponding channel register
	return ((*(_Periph + 4 + Output_Channel) & 0x0000FFFF) >> 4);
}

ADC::ADC(uint8_t Ch, uint32_t Target_Freq)
{
	PCONP |= (1 << 12); // Set bit in PCONP
	*_Periph |= (1 << 21); // Set PDN bit in ADCR to enable ADC peripheral

	PCLKSEL0 &= ~(3 << 24); // Set CCLK/4 clock rate
	*_Periph |= ((((SYS_FREQ >> 2) / Target_Freq) - 1) << 8); // Set CLKDIV in ADCR

	Output_Channel = Ch;
}
/***********************************************************************************************************/
uint32_t * DAC::_Periph = (uint32_t *)0x4008C000;

void DAC::Set_Output(uint16_t Val_10bit)
{
	*_Periph &= 0x10000; // Clear BIAS bit in DACR
	*_Periph |= (Val_10bit << 6); // Set value to be converted in DACR
}

void DAC::Set_Out_Percent(uint8_t Percentage)
{
	uint32_t Val = (Percentage << 10) / 100;

	Set_Output((uint16_t)Val);
}
/***********************************************************************************************************/
void _I2C::Init(uint8_t Bus_Mode, uint32_t I2C_Speed)
{
	PCLKSEL0 &= ~(3 << Clk_bit);
	PCLKSEL0 |= (CLK0 << Clk_bit);

	*(_Periph + 4) = I2C_Speed; // SCLH for setting bus frequency
	*(_Periph + 5) = I2C_Speed; // SCLL (SCLL and SCLH are the same value for 50% duty cycle)

	*(_Periph +1) = 0x00;

	EnableIRQ(IRQ_bit);

	*_Periph = Bus_Mode; // Set I2CONSET for slave or master mode
}

uint8_t _I2C::get_Status()
{
	return *(_Periph + 1); // I2STAT
}

void _I2C::New_Sequence(uint8_t Slave_Address, uint16_t readLength)
{
	for(uint16_t i = 0; i < 512; i++){ // Reset Buffer, length and master counter
		tx_buf[i] = 0;
		rx_buf[i] = 0;
	}

	Read_Length = readLength;
	Msg_Length = 0;
	M_Counter_tx = 0;
	M_Counter_rx = 0;

	tx_buf[Msg_Length++] = Slave_Address;
}

void _I2C::Add_String(const char * StrToSend)
{
	while(*StrToSend != '\0'){
		tx_buf[Msg_Length++] = *StrToSend++;
	}
}

void _I2C::Add_BCD_Integer(uint32_t Decimal_uint, uint8_t NbDigits)
{
	IntegerToBCD(LCD_BCD, Decimal_uint);

	switch(NbDigits){
	case 4:
		tx_buf[Msg_Length++] = (LCD_BCD.mil + 48);
	case 3:
		tx_buf[Msg_Length++] = (LCD_BCD.cen + 48);
	case 2:
		tx_buf[Msg_Length++] = (LCD_BCD.diz + 48);
	case 1:
		tx_buf[Msg_Length++] = (LCD_BCD.uni + 48);
		break;
	}

}

void _I2C::Add_BCD_Float(uint32_t Decimal_uint, uint8_t NbDigits, uint8_t DP_Position)
{
	IntegerToBCD(LCD_BCD, Decimal_uint);

	switch(NbDigits){
	case 4:
		tx_buf[Msg_Length++] = (LCD_BCD.mil + 48);
		if(DP_Position == 1)
			tx_buf[Msg_Length++] = '.';
	case 3:
		tx_buf[Msg_Length++] = (LCD_BCD.cen + 48);
		if(DP_Position == 2)
			tx_buf[Msg_Length++] = '.';
	case 2:
		tx_buf[Msg_Length++] = (LCD_BCD.diz + 48);
		if(DP_Position == 3)
			tx_buf[Msg_Length++] = '.';
	case 1:
		tx_buf[Msg_Length++] = (LCD_BCD.uni + 48);
		break;
	}
}

void _I2C::Add_Byte(uint8_t ByteToSend)
{
	tx_buf[Msg_Length++] = ByteToSend;
}

void _I2C::Read_Next_Byte()
{
	rx_buf[M_Counter_rx++] = *(_Periph + 2);// Get I2DAT byte

	*(_Periph + 6) = 0x08; // Clear SI in I2CONCLR
}

void _I2C::Send_Next_Byte()
{
	*(_Periph + 2) = tx_buf[M_Counter_tx++]; // Put next byte in I2DAT

	*(_Periph + 6) = 0x08; // Clear SI in I2CONCLR
}

void _I2C::Send_Address()
{
	*(_Periph + 2) = tx_buf[M_Counter_tx++]; // Put address in I2DAT

	*(_Periph + 6) = 0x28; // Clear STA and SI in I2CONCLR
}

void _I2C::Send_Stop()
{
	*_Periph |= 0x10; // Set STO in I2CONSET

	*(_Periph + 6) = 0x48; // Clear SI and I2EN in I2CONCLR

	Busy = false;
}

void _I2C::Send_nACK()
{
	*(_Periph + 6) = 0x0C; // Clear SI and AA in I2CONCLR
}

void _I2C::Send_Sequence()
{
	Busy = true;

	*_Periph |= 0x64; // Set STA,I2EN and AA in I2CONSET
}

void _I2C::Clear_Interrupt()
{
	*(_Periph + 6) = 0x08; // Clear SI in I2CONCLR
}

_I2C::_I2C(uint32_t * PtrI2C, uint8_t C_bit, uint8_t Int_bit)
{
	_Periph = PtrI2C;
	Clk_bit = C_bit;
	IRQ_bit = Int_bit;
}
/***********************************************************************************************************/
void SSP::Init(uint8_t ClkPrescaler, uint8_t DataSizeSelect, uint8_t CPOL, uint8_t CPHA)
{
	if(Clk_bit < 16){ // SSP0
		PCLKSEL1 &= ~(3 << Clk_bit);
		PCLKSEL1 |= (CLK0 << Clk_bit);
	}
	else{ // SSP1 & Legacy SPI
		PCLKSEL0 &= ~(3 << Clk_bit);
		PCLKSEL0 |= (CLK0 << Clk_bit);
	}

	*(_Periph + 4) = ClkPrescaler; // Set CPSDVSR

	*_Periph = DataSizeSelect - 1; // 4 to 16 bits transfer
	*_Periph |= CPOL << 6; // Clock polarity select
	*_Periph |= CPHA << 7; // Clock phase select

	*_Periph &= ~(0xFF << 8); // 0 in SCR for 377KHz to 48MHz SPI rate

	*(_Periph + 5) = 0; // Clear enabled interrupts

	*(_Periph + 9) = (1 << 1);

	*(_Periph + 1) = 0x2; // Enable SSPx controller
}

void SSP::New_Sequence()
{
	for(uint8_t i = 0; i < 100; i++){ // Reset Buffer, length and master counter
		Buffer[i] = 0;
	}
	Msg_Length = 0;
	M_Counter = 0;
}

void SSP::Add_String(const char * StrToSend)
{
	while(*StrToSend != '\0'){
		Buffer[Msg_Length++] = *StrToSend++;
	}
}

void SSP::Add_BCD_Integer(uint32_t Decimal_uint, uint8_t NbDigits)
{
	IntegerToBCD(LCD_BCD, Decimal_uint);

	switch(NbDigits){
	case 4:
		Buffer[Msg_Length++] = (LCD_BCD.mil + 48);
	case 3:
		Buffer[Msg_Length++] = (LCD_BCD.cen + 48);
	case 2:
		Buffer[Msg_Length++] = (LCD_BCD.diz + 48);
	case 1:
		Buffer[Msg_Length++] = (LCD_BCD.uni + 48);
		break;
	}

}

void SSP::Add_BCD_Float(uint32_t Decimal_uint, uint8_t NbDigits, uint8_t DP_Position)
{
	IntegerToBCD(LCD_BCD, Decimal_uint);

	switch(NbDigits){
	case 4:
		Buffer[Msg_Length++] = (LCD_BCD.mil + 48);
		if(DP_Position == 1)
			Buffer[Msg_Length++] = '.';
	case 3:
		Buffer[Msg_Length++] = (LCD_BCD.cen + 48);
		if(DP_Position == 2)
			Buffer[Msg_Length++] = '.';
	case 2:
		Buffer[Msg_Length++] = (LCD_BCD.diz + 48);
		if(DP_Position == 3)
			Buffer[Msg_Length++] = '.';
	case 1:
		Buffer[Msg_Length++] = (LCD_BCD.uni + 48);
		break;
	}
}

void SSP::Add_Byte(uint16_t ByteToSend)
{
	Buffer[Msg_Length++] = ByteToSend;
}

void SSP::Send_Next_Byte()
{
	if(Msg_Length - M_Counter >= 8){
		for(int i = 0; i < 7; i++){
			*(_Periph + 2) = Buffer[M_Counter++]; // Put next byte in SSPDR
		}
	}
	else{
		for(int i = 0; i < (Msg_Length - M_Counter); i++){
			*(_Periph + 2) = Buffer[M_Counter++]; // Put next byte in SSPDR
		}
	}
}

void SSP::Send_Sequence()
{
	Enable_Int(TXIM);
}

void SSP::End_Transfer()
{
	Disable_Int(TXIM);
}

void SSP::Enable_Int(uint8_t IntSelect)
{
	*(_Periph + 5) |= (1 << IntSelect); // Enable selected interrupt

	EnableIRQ(IRQ_bit);
}

void SSP::Disable_Int(uint8_t IntSelect)
{
	*(_Periph + 5) &= ~(1 << IntSelect); // Disable selected interrupt
}

SSP::SSP(uint32_t * PtrSSP, uint8_t C_bit, uint8_t Int_bit)
{
	_Periph = PtrSSP;
	Clk_bit = C_bit;
	IRQ_bit = Int_bit;
}
