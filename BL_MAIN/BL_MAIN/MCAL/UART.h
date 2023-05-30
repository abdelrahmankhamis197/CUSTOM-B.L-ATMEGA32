
#ifndef UART_H_
#define UART_H_
//Speed_Mode
#define		UART_SPEED_MODE		UART_NORMAL_SPEED
#define		UART_DOUBLE_SPEED	1
#define		UART_NORMAL_SPEED	0

//TX_RX Status
#define		TX_MODE			TX_ENABLE
#define		TX_ENABLE		1
#define		TX_DISABLE		0

#define		RX_MODE			RX_ENABLE
#define		RX_ENABLE		1
#define		RX_DISABLE		0

//SET_Data_Len
typedef enum
{
	DATA_BITS_5=0,
	DATA_BITS_6,
	DATA_BITS_7,
	DATA_BITS_8,
	DATA_BITS_9=7
}Data_Len;

void	UART_Init(void);

void	UART_SendUsingPooling(u8 data);
u8		UART_ReceiveUsingpooling(void);

void	UART_Send_NoBlock(u8 data);
u8		UART_ReceiveByteNoBlock(void);

u8		UART_SendReceive(u8 data);

void	UART_TxInterruptEnable(void);
void	UART_RX_InterruptEnable(void);

void	UART_TxInterruptDisable(void);
void    UART_RX_InterruptDisable(void);

void    UART_RX_SetCallBack(void(*pfn)(void));
void    UART_TX_SetCallBack(void(*pfn)(void));

#endif /* UART_H_ */