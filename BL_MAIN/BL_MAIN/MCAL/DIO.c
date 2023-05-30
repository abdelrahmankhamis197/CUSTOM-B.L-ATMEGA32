#include "MEMMAP.h"
#include "UTILS.h"
#include "STDTYPES.h"
#include "DIO_Interface.h"

extern const DIO_PinStatus_type PinsStatusArray[TOTAL_PINS];

void DIO_InitPin(DIO_Pin_type pin,DIO_PinStatus_type status)
{
	DIO_Port_type port;
	DIO_PortPin_t pin_num;
	port=pin/8;
	pin_num=pin%8;
	
	switch(status)
	{
		case OUTPUT:
		switch(port)
		{
			case PA:
			SET_BIT(DDRA,pin_num);
			break;
			
			case PB:
			SET_BIT(DDRB,pin_num);
			break;
			
			case PC:
			SET_BIT(DDRC,pin_num);
			break;
			
			case PD:
			SET_BIT(DDRD,pin_num);
			break;
		}
		break;
		case INFREE:
		switch(port)
		{
			case PA:
			CLR_BIT(DDRA,pin_num);
			CLR_BIT(PORTA,pin_num);
			break;
			
			case PB:
			CLR_BIT(DDRB,pin_num);
			CLR_BIT(PORTB,pin_num);
			break;
			
			case PC:
			CLR_BIT(DDRC,pin_num);
			CLR_BIT(PORTC,pin_num);
			break;
			
			case PD:
			CLR_BIT(DDRD,pin_num);
			CLR_BIT(PORTD,pin_num);
			break;
		}
		break;
		case INPULLUP:
		switch(port)
		{
			case PA:
			CLR_BIT(DDRA,pin_num);
			SET_BIT(PORTA,pin_num);
			break;
			
			case PB:
			CLR_BIT(DDRB,pin_num);
			SET_BIT(PORTB,pin_num);
			break;
			
			case PC:
			CLR_BIT(DDRC,pin_num);
			SET_BIT(PORTC,pin_num);
			break;
			
			case PD:
			CLR_BIT(DDRD,pin_num);
			SET_BIT(PORTD,pin_num);
			break;
		}
		break;
	}
}

void DIO_Init(void)
{
	DIO_Pin_type i;
	for(i=PINA0;i<TOTAL_PINS;i++)
	{
		DIO_InitPin(i,PinsStatusArray[i]);
	}
}

void DIO_InitPort(DIO_Port_type port,DIO_PinStatus_type status,u8 mask)
{
	switch(status)
	{
		case OUTPUT:
		switch(port)
		{
			case PA:
			SET_PORT(DDRA,mask);
			break;
			
			case PB:
			SET_PORT(DDRB,mask);
			break;
			
			case PC:
			SET_PORT(DDRC,mask);
			break;
			
			case PD:
			SET_PORT(DDRD,mask);
			break;
		}
		break;
		case INFREE:
		switch(port)
		{
			case PA:
			CLR_PORT(DDRA);
			CLR_PORT(PORTA);
			break;
			
			case PB:
			CLR_PORT(DDRB);
			CLR_PORT(PORTB);
			break;
			
			case PC:
			CLR_PORT(DDRC);
			CLR_PORT(PORTC);
			break;
			
			case PD:
			CLR_PORT(DDRD);
			CLR_PORT(PORTD);
			break;
		}
		break;
		case INPULLUP:
		switch(port)
		{
			case PA:
			CLR_PORT(DDRA);
			SET_PORT(PORTA,mask);
			break;
			
			case PB:
			CLR_PORT(DDRB);
			SET_PORT(PORTB,mask);
			break;
			
			case PC:
			CLR_PORT(DDRC);
			SET_PORT(PORTC,mask);
			break;
			
			case PD:
			CLR_PORT(DDRD);
			SET_PORT(PORTD,mask);
			break;
		}
		break;
	}
}


DIO_Voltage_type DIO_ReadPin(DIO_Pin_type pin)
{
	DIO_Port_type port;
	DIO_PortPin_t pin_num;
	port=pin/8;
	pin_num=pin%8;
	switch(port)
	{
		case PA:
		return READ_BIT(PINA,pin_num);
		break;
		case PB:
		return READ_BIT(PINB,pin_num);
		break;
		case PC:
		return READ_BIT(PINC,pin_num);
		break;
		case PD:
		return READ_BIT(PIND,pin_num);
		break;
	}
	return 0;
}

u8 DIO_ReadPort(DIO_Port_type port)
{
	switch(port)
	{
		case PA:
			return PINA;
			break;
		case PB:
			return PINB;
			break;
		case PC:
			return PINC;
			break;
		case PD:
			return PIND;
			break;
	}
	return 10;
}


void DIO_WritePin(DIO_Pin_type pin,DIO_Voltage_type volt)
{
	DIO_Port_type port;
	DIO_PortPin_t pin_num;
	port=pin/8;
	pin_num=pin%8;
	switch(port)
	{
		case PA:
		switch(volt)
		{
			case HIGH:
			SET_BIT(PORTA,pin_num);
			break;
			case LOW:
			CLR_BIT(PORTA,pin_num);
			break;
		}
		break;
		case PB:
		switch(volt)
		{
			case HIGH:
			SET_BIT(PORTB,pin_num);
			break;
			case LOW:
			CLR_BIT(PORTB,pin_num);
			break;
		}
		break;
		case PC:
		switch(volt)
		{
			case HIGH:
			SET_BIT(PORTC,pin_num);
			break;
			case LOW:
			CLR_BIT(PORTC,pin_num);
			break;
		}
		break;
		case PD:
		switch(volt)
		{
			case HIGH:
			SET_BIT(PORTD,pin_num);
			break;
			case LOW:
			CLR_BIT(PORTD,pin_num);
			break;
		}
		break;
	}
}


void DIO_WritePort(DIO_Port_type port,u8 mask)
{
	switch(port)
	{
		case PA:
		PORTA=mask;
		break;
		case PB:
		PORTB=mask;
		break;
		case PC:
		PORTC=mask;
		break;
		case PD:
		PORTD=mask;
		break;
	}
}


void DIO_TogglePin(DIO_Pin_type pin)
{
	DIO_Port_type port;
	DIO_PortPin_t pin_num;
	port=pin/8;
	pin_num=pin%8;
	switch(port)
	{
		case PA:
		TOGGLE_BIT(PORTA,pin_num);
		break;
		
		case PB:
		TOGGLE_BIT(PORTB,pin_num);
		break;
		
		case PC:
		TOGGLE_BIT(PORTC,pin_num);
		break;
		
		case PD:
		TOGGLE_BIT(PORTD,pin_num);
		break;
	}
}

void DIO_TogglePort(DIO_Port_type port)
{
	switch(port)
	{
		case PA:
		TOGGLE_PORT(PORTA);
		return;
		case PB:
		TOGGLE_PORT(PORTB);
		return;
		case PC:
		TOGGLE_PORT(PORTC);
		return;
		case PD:
		TOGGLE_PORT(PORTD);
		return;
	}
}

// void DIO_Disable_Pullup(void)
// {
// 	SET_BIT(SFIOR,PUD);
// }