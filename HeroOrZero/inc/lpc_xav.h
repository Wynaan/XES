#include "chip.h"
#include "board.h"
#include "cr_section_macros.h"

#ifndef LPC_XAV_H_
#define LPC_XAV_H_

#define PCONP	 *((uint32_t *)0x400FC0C4)
#define PCLKSEL0 *((uint32_t *)0x400FC1A8)
#define PCLKSEL1 *((uint32_t *)0x400FC1AC)

#define I2C_400KHZ	120
#define I2C_100KHZ 	480
#define I2C_50KHZ 	960
#define SPI_24MHZ	4
#define SPI_100KHZ  240

#define SYS_FREQ ((uint32_t)96000000)

enum {GPIO, Fct0x1, Fct0x2, Fct0x3}; 				// GPIO pin function
enum {IN, OUT};										// GPIO direction
enum {Pullup, Repeater, Float, Pulldown, Default};	// GPIO input mode
enum {Normal, Open_Drain}; 							// GPIO output mode
enum {Level_Sensitive, Edge_Sensitive};				// EXT int mode
enum {UND2, Rising_Edge, Falling_Edge, Both_Edges};	// Timer capture setting
enum {CLK4, CLK0, CLK2, CLK8};						// Timer clock rate (number stands for divisor)
enum {Nothing, Clear, Set, Toggle};					// External match setting
enum {MR0 = 0x1, MR1 = 0x2, MR2 = 0x4, MR3 = 0x8, CR0 = 0x10, CR1 = 0x20};// Timer interrupt flag bits order
enum {Disabled, Odd, Even = 3};						// UART Parity control
enum {Single_Edge, Double_Edge};					// Motor PWM mode
enum {Manual, Auto};								// AD Converter mode for starting conversions
enum {Master = 0x40, Slave = 0x44};					// I2C Mode
enum {RORIM, RTIM, RXIM, TXIM};						// SSP Interrupt sources

class LPC_IO;

void EnableIRQ(uint8_t Int_bit); // Enables periph's NVIC interrupt
void GPIO_Enable_Int(uint8_t PortNo, uint8_t PinNo, uint8_t Edge_Select);
void Enable_EXT_Int(uint8_t ExtIntNo, uint8_t ExtMode);
/***********************************************************************************************************/
class Port{ // Port object to provide pointers for the Lpc_IO class
	friend LPC_IO;
	uint32_t * _PINCB; 	// Address of the first PINSEL register for desired port
	uint32_t * _FIO; 	// Address of the first FIO register
	uint32_t * _OD;		// Address of the open drain mode register
	void SetFct(uint8_t PinNo, uint8_t Fct);
	void SetDir(uint8_t PinNo, uint8_t Dir);
	void SetMode(uint8_t PinNo, uint8_t Mode);
	void SetOpenDrain(uint8_t PinNo, uint8_t Mode );

public:
	Port(uint32_t* Pinsel_Add, // Constructor for LPC Port handles
		 uint32_t* FastIO_Add,
		 uint32_t* OpenDrain_Add);
};
/***********************************************************************************************************/
class LPC_IO{ // Pin object to initialize all the register required for a pin functioning
	Port& _Port;
	uint8_t Pin;
	uint8_t Function;
	uint8_t Direction;
	uint8_t Mode;
	uint8_t OD_Mode;

public:
	void Out(bool State);
	void Set();
	void Clear();

	LPC_IO();
	LPC_IO(	Port& PtrPort, // Constructor for IO pin handles.
			uint8_t PinNo,
			uint8_t Fct,
			uint8_t Dir = OUT,
			uint8_t Md = Float,
			uint8_t OD = Normal);

	void operator=(bool OutputState) // Allows to use Object = boolean value; to set pin output
	{
		this->Out(OutputState);
	}
	bool operator==(bool BooleanValue)
	{
		if(BooleanValue == ((*(_Port._FIO + 5) >> this->Pin) & 0x1)){
			return true;
		}
		else{
			return false;
		}
	}
	bool operator!=(bool BooleanValue)
	{
		if(BooleanValue == ((*(_Port._FIO + 5) >> this->Pin) & 0x1)){
			return false;
		}
		else{
			return true;
		}
	}
};
/***********************************************************************************************************/
class Bus{ // Bus object can be declared to group Lpc_IO handles together and use it as one 8-bit register
	LPC_IO * Bits[8];
	uint8_t Value;

public:
	void Change_Value(uint8_t bus_value);

	Bus(LPC_IO* LSB=nullptr, LPC_IO* B1=nullptr,
		LPC_IO* B2=nullptr, LPC_IO* B3=nullptr,
		LPC_IO* B4=nullptr, LPC_IO* B5=nullptr,
		LPC_IO* B6=nullptr, LPC_IO* MSB=nullptr);

	void operator=(uint8_t bus_value)
	{
		Change_Value(bus_value);
	}
	void operator+=(uint8_t increment_value)
	{
		Change_Value(Value + increment_value);
	}
};
/***********************************************************************************************************/
class Timer{ // Timer object to group together all of a timer's functions and registers
	uint32_t * _Periph;
	uint8_t Pwr_bit;
	uint8_t Clk_bit;
	uint8_t IRQ_bit;

public:
	uint8_t Edge_Sel[2];
	void Init(uint8_t Clk_Speed); // Uses the IRQ bit to determine which PCLKSEL to set
	void Enable_Counter(uint8_t Channel, uint8_t Edge_Mode);
	void Enable_Capture(uint8_t Channel, uint8_t Edge_Mode);
	void Reset_onMatch(uint8_t Channel);
	void Set_Match(uint8_t Channel, uint32_t Match_Value);
	void Enable_Ext_Match(uint8_t Channel, uint8_t Match_Setting);
	void Enable_Mat_Int(uint8_t Channel);
	void Enable_Cap_Int(uint8_t Channel);
	void Reset(); // Resets the TC
	uint8_t get_IR();
	void Clear_Int(uint8_t FlagBit);
	uint32_t get_CR(uint8_t Channel);

	Timer(	uint32_t * PtrTimer, // Constructor function for timers
			uint8_t P_bit,
			uint8_t C_bit,
			uint8_t Int_bit);
};
/***********************************************************************************************************/
class UART{
	uint32_t * _Periph;
	uint8_t Pwr_bit;
	uint8_t Clk_bit;
	uint8_t IRQ_bit;

public:
	void Init(uint32_t Baudrate);
	void Set_Transmission(uint8_t Bit_Length, uint8_t Parity);
	void Enable_RX_Int();
	void PutByte(char Byte);
	void PrintStrConst(const char *StrToSend);
	void PrintDec(uint16_t NbrDec);
	void PrintFloat(uint16_t Scaled_Int);
	void ClearScreen();
	bool Int_Flag_Set();
	uint8_t get_RBR();

	UART(uint32_t * PtrUART, uint8_t P_bit, uint8_t C_bit, uint8_t Int_bit);
};
/***********************************************************************************************************/
class PWM{
	static uint32_t * _Periph;
	uint8_t Output_Channel;
public:
	float Duty;
	static void Set_Freq(uint32_t Freq); // Setup MR0 value that controls frequency of all single-e PWMs
	void Reset_onMatch();
	void Set_Duty(float DutyPercentage);
	void Increment_Duty();
	void Decrement_Duty();
	static void Enable(); // Always use at the end of initialization, starts TC for all PWMs

	PWM(uint8_t Ch, uint8_t Edge_Sel);
};
/***********************************************************************************************************/
class ADC{
	static uint32_t * _Periph;
	uint8_t Output_Channel;
public:
	static void Start_Burst_Conversion(uint8_t Sel_Val);
	static void Stop_Burst_Conversion();
	void Start_Conversion();
	void Stop_Conversion();
	void Enable_Int();
	uint16_t get_12bit_Result();

	ADC(uint8_t Ch, uint32_t Target_Freq);
};
/***********************************************************************************************************/
class DAC{
	static uint32_t * _Periph;
public:
	void Set_Output(uint16_t Val_10bit);
	void Set_Out_Percent(uint8_t Percentage);
};
/***********************************************************************************************************/
class _I2C{
	uint32_t * _Periph;
	uint8_t Clk_bit;
	uint8_t IRQ_bit;
public:
	uint8_t tx_buf[512];
	uint8_t rx_buf[512];
	bool Busy;
	uint8_t M_Counter_tx;
	uint8_t M_Counter_rx;
	uint8_t Msg_Length;
	uint8_t Read_Length;

	void Init(uint8_t Bus_Mode, uint32_t I2C_Speed);
	uint8_t get_Status();
	void New_Sequence(uint8_t Slave_Address, uint16_t readLength = 0);
	void Add_String(const char * StrToSend);
	void Add_BCD_Integer(uint32_t NumberToSend, uint8_t NbDigits);
	void Add_BCD_Float(uint32_t NumberToSend, uint8_t NbDigits, uint8_t DP_Position);
	void Add_Byte(uint8_t ByteToSend);
	void Send_Next_Byte();
	void Read_Next_Byte();
	void Send_Address();
	void Send_Stop();
	void Send_nACK();
	void Send_Sequence();
	void Clear_Interrupt();

	_I2C(uint32_t * PtrI2C, uint8_t C_bit, uint8_t Int_bit);
};
/***********************************************************************************************************/
class SSP{
	uint32_t * _Periph;
	uint8_t Clk_bit;
	uint8_t IRQ_bit;
	uint16_t Buffer[128];
public:
	uint32_t MIS = *(_Periph + 7);
	uint8_t M_Counter;
	uint8_t Msg_Length;

	void Init(uint8_t ClkPrescaler, uint8_t DataSizeSelect, uint8_t CPOL, uint8_t CPHA);
	void New_Sequence();
	void Add_String(const char * StrToSend);
	void Add_BCD_Integer(uint32_t NumberToSend, uint8_t NbDigits);
	void Add_BCD_Float(uint32_t NumberToSend, uint8_t NbDigits, uint8_t DP_Position);
	void Add_Byte(uint16_t ByteToSend);
	void Send_Next_Byte();
	void Send_Sequence();
	void End_Transfer();
	void Enable_Int(uint8_t IntSelect);
	void Disable_Int(uint8_t IntSelect);

	SSP(uint32_t * PtrSSP, uint8_t C_bit, uint8_t Int_bit);
};
/***********************************************************************************************************/
// Mandatory objects declaration (Peripherals)
static Port P0((uint32_t *)0x4002C000, (uint32_t *)0x2009C000, (uint32_t *)0x4002C068);
static Port P1((uint32_t *)0x4002C008, (uint32_t *)0x2009C020, (uint32_t *)0x4002C06C);
static Port P2((uint32_t *)0x4002C010, (uint32_t *)0x2009C040, (uint32_t *)0x4002C070);
static Port P3((uint32_t *)0x4002C018, (uint32_t *)0x2009C060, (uint32_t *)0x4002C074);
static Port P4((uint32_t *)0x4002C020, (uint32_t *)0x2009C080, (uint32_t *)0x4002C078);

static Timer TMR0((uint32_t *)0x40004000, 1, 2, 1);
static Timer TMR1((uint32_t *)0x40008000, 2, 4, 2);
static Timer TMR2((uint32_t *)0x40090000, 22, 12, 3);
static Timer TMR3((uint32_t *)0x40094000, 23, 14, 4);

static UART UART0((uint32_t *)0x4000C000, 3, 6, 5);
static UART Keyboard((uint32_t *)0x40098000, 24, 16, 7);
static UART Console((uint32_t *)0x4009C000, 25, 18, 8);

#endif /* LPC_XAV_H_*/
