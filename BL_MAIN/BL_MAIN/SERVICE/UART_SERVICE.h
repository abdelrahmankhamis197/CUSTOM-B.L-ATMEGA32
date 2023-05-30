

#ifndef UART_SERVICE_H_
#define UART_SERVICE_H_

void UART_SendString(u8 *pdata);

void UART_RecieveString(u8 *str);

void UART_SendNumber(u32 num);

u32 UART_RecieveNumber(void);

// bool_type UART_String_Compare(u8*str1,u8*str2);
// void UART_split_string(u8*recieved_str,u8* split_str,u8* num);



#endif /* UART_SERVICE_H_ */