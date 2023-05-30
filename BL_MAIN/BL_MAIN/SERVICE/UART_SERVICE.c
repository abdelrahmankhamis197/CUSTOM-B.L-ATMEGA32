#include "STDTYPES.h"
#include "UART.h"
#include "UART_SERVICE.h"

void UART_SendString(u8 *pdata)
{
	u8 i;
	for(i=0;pdata[i];i++)
	{
		UART_SendUsingPooling(pdata[i]);
	}
	UART_SendUsingPooling('.');
}

void UART_RecieveString(u8 *str)
{
	u8 i=0;
	str[i]=UART_ReceiveUsingpooling();
	while(str[i] != '.')
	{
		i++;
		str[i]=UART_ReceiveUsingpooling();
	}
	str[i]=0;
	
}

void UART_SendNumber(u32 num)
{													//0xaaffcc00
	UART_SendUsingPooling( (u8) (num)    );			//00
	UART_SendUsingPooling( (u8) (num>>8) );		//cc
	UART_SendUsingPooling( (u8) (num>>16) );	//ff
	UART_SendUsingPooling( (u8) (num>>24) );	//aa
}

u32 UART_RecieveNumber(void)
{
	u32 num;
	u8 b1,b2,b3,b4;
	b1=UART_ReceiveUsingpooling();
	b2=UART_ReceiveUsingpooling();
	b3=UART_ReceiveUsingpooling();
	b4=UART_ReceiveUsingpooling();
	num=( (u32)b1|(u32)b2<<8|(u32)b3<<16|(u32)b4<<24 );
	return num;
}

// 
// bool_type UART_String_Compare(u8*str1,u8*str2)
// {
// 	u8 i;
// 	for(i=0;str1[i] || str2[i] ;i++)
// 	{
// 		if( (str2[i] >= 'A') && (str2[i] <= 'Z') )
// 		{
// 			str2[i]=(str2[i]-'A')+'a';
// 		}
// 	}
// 
// 	for(i=0; str1[i] || str2[i] ;i++)
// 	{
// 		if( str1[i]!=str2[i] )
// 		{
// 			return FALSE;
// 		}
// 	}
// 	return TRUE;
// void UART_split_string(u8*recieved_str,u8* split_str,u8* num) //
// {
// 	u8 i=0,char_index=0;
// 	for(i=0;recieved_str[i];i++)
// 	{
// 		if( (recieved_str[i] >= 'a') && (recieved_str[i] <= 'z') )
// 		{
// 			char_index++;
// 		}
// 		else if((recieved_str[i] >= '0') && (recieved_str[i] <= '9'))
// 		{
// 			*num =(recieved_str[i]-'0')+0;
// 		}
// 	}
// 	
// 	for(i=0;i<char_index;i++)
// 	{
// 		split_str[i]=recieved_str[i];
// 	}
// 	split_str[i]=0;
// 	
// }
