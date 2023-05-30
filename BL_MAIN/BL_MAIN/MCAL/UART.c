#include "MEMMAP.h"
#include "UTILS.h"
#include "STDTYPES.h"
#include "UART.h" 

void UART_Init(void)
{
	//select_Speed_Mode
	#if(UART_SPEED_MODE		==	UART_NORMAL_SPEED)
	CLR_BIT(UCSRA,U2X);	/*using uart in normal speed mode*/
	#elif(UART_SPEED_MODE	==	UART_DOUBLE_SPEED)
	SET_BIT(UCSRA,U2X);	/*using uart in double speed mode*/
	#endif
	
	UBRRL=51;	/*configure uart buadrate*/
	
	//enable UART_TX
	#if(TX_MODE==TX_ENABLE)
	SET_BIT(UCSRB,TXEN);	/*activate tx_uart*/
	#elif(TX_MODE==TX_DISABLE)
	CLR_BIT(UCSRB,TXEN);	/*deactivate tx_uart*/
	#endif
	//enable UART_RX
	#if(RX_MODE==RX_ENABLE)
	SET_BIT(UCSRB,RXEN);	/*activate rx_uart*/
	#elif(RX_MODE==RX_DISABLE)
	CLR_BIT(UCSRB,RXEN);	/*deactivate rx_uart*/
	#endif
}

/****send _recieve using pooling*******/

void UART_SendUsingPooling(u8 data)
{
	while(!READ_BIT(UCSRA,UDRE));
	UDR=data;
}

u8 UART_ReceiveUsingpooling(void)
{
	while(!READ_BIT(UCSRA,RXC));
	return	UDR;
}
/*********************************/

/*___________________________________________*/


/*******send_recieve no block****/

void UART_Send_NoBlock(u8 data)
{
	UDR=data;
}

u8 UART_ReceiveByteNoBlock(void)
{
	return UDR;
}
/*********************************/

/*****************************************/
u8 UART_SendReceive(u8 data)
{
	UDR=data;
	while(!READ_BIT(UCSRA,UDRE));
	return UDR;
}
/*****************************************/


/* INterrupts TX,Rx enable */

static void(*Tx_Ptr)(void)=NULLPTR;
static void(*Rx_Ptr)(void)=NULLPTR;
/* Enable*/
void UART_TxInterruptEnable(void)
{
	SET_BIT(UCSRB,TXCIE);
}

void UART_RX_InterruptEnable(void)
{
	SET_BIT(UCSRB,RXCIE);
}
/* disable*/
void UART_TxInterruptDisable(void)
{
	CLR_BIT(UCSRB,TXCIE);
}

void UART_RX_InterruptDisable(void)
{
	CLR_BIT(UCSRB,RXCIE);
}

/*set callback*/

void UART_RX_SetCallBack(void(*pfn)(void))
{
	Rx_Ptr=pfn;
}
void UART_TX_SetCallBack(void(*pfn)(void))
{
	Tx_Ptr=pfn;
}

/* RX___Tx ISRs */

ISR(UART_RX_vect)
{
	if(Rx_Ptr!=NULLPTR)
	{
		Rx_Ptr();
	}
	
}
ISR(UART_TX_vect)
{
	if(Tx_Ptr!=NULLPTR)
	{
		Tx_Ptr();
	}
}