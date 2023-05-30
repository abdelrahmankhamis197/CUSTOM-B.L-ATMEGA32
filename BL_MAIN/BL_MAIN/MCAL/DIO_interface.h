
#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_
#include "MEMMAP.h"
#include "UTILS.h"
#include "STDTYPES.h"

typedef enum
{
	OUTPUT=0,
	INFREE,
	INPULLUP
}DIO_PinStatus_type;

typedef enum
{
	PA=0,
	PB,
	PC,
	PD
}DIO_Port_type;




typedef enum
{
	PINA0=0,
	PINA1=1,
	PINA2,
	PINA3,
	PINA4,
	PINA5,
	PINA6,
	PINA7,
	PINB0,
	PINB1,
	PINB2,
	PINB3,
	PINB4,
	PINB5,
	PINB6,
	PINB7,
	PINC0,
	PINC1,
	PINC2,
	PINC3,
	PINC4,
	PINC5,
	PINC6,
	PINC7,
	PIND0,
	PIND1,
	PIND2,
	PIND3,
	PIND4,
	PIND5,
	PIND6,
	PIND7,
	TOTAL_PINS
}DIO_Pin_type;

typedef enum
{
	LOW=0,
	HIGH=1
	
}DIO_Voltage_type;


typedef enum
{
	pin0=0,
	pin1,
	pin2,
	pin3,
	pin4,
	pin5,
	pin6,
	pin7
}DIO_PortPin_t;

void DIO_Init(void);
u8 DIO_ReadPort(DIO_Port_type port);
void DIO_InitPin(DIO_Pin_type pin,DIO_PinStatus_type status);
void DIO_InitPort(DIO_Port_type port,DIO_PinStatus_type status,u8 mask);
void DIO_WritePin(DIO_Pin_type pin,DIO_Voltage_type volt);
DIO_Voltage_type DIO_ReadPin(DIO_Pin_type pin);
void DIO_WritePort(DIO_Port_type port,u8 mask);
void DIO_TogglePin(DIO_Pin_type pin);
void DIO_TogglePort(DIO_Port_type port);
//void DIO_Disable_Pullup(void);
#endif /* DIO_INTERFACE_H_ */