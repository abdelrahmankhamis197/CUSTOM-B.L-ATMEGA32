#define  F_CPU 8000000
#include "STDTYPES.h"
//#include "MEMMAP.h"
#include "UTILS.h"

#include "DIO_interface.h"
#include "EEPROM.h"
#include "UART.h"
#include "FLASHING_Manager.h"

#define RED_LED   PINC0
#define GREEN_LED PINC1
#define BLUE_LED  PINC2

int main(void)
{
    DIO_Init();
	//UART_Init();
	// MOVING THE VEC TABLE
	UART_RX_SetCallBack(UART_RX_Complete);
	//UART_RX_InterruptEnable();
	// sei();
	
	u8 validAPP_Val, reqFromApp_Val;
	validAPP_Val = EEPROM_ReadData (VALID_APP_ADDRESS);
	reqFromApp_Val = EEPROM_ReadData (REQ_FROM_APP_ADDRESS);
	
	/******************************************************************/
    if ( (validAPP_Val == EEPROM_BYTE_NOT_SET) | (reqFromApp_Val == EEPROM_BYTE_SET))
    {
	    /*for debugging*/
	    DIO_WritePin(RED_LED,HIGH);
	    DIO_WritePin(BLUE_LED,HIGH);
	    UART_Init();
	     DIO_WritePin(GREEN_LED,HIGH);
	    #if USE_INTERRUPT == 1
	    /*Move Vector Table to boot loader section*/
	    APP_vidMoveIVT(BLD_SECTION);
		UART_RX_InterruptEnable();
		sei();
	    #endif

	    if(reqFromApp_Val == EEPROM_BYTE_SET)
	    {
		    flashingMngr_vidHandleReqFromApp() ;
		    /*clr application request*/
		   EEPROM_WriteData(REQ_FROM_APP_ADDRESS, EEPROM_BYTE_NOT_SET);
	    }

	    while(1)
	    {
		    #if USE_INTERRUPT == 0
		      UART_RX_Complete();
		    #endif
		    flashingMngr_vidMainTask() ;
	    }
    }
    else
    {
	    /*Valid app, jump directly*/
	    asm("jmp 0");
    }

    return 1 ;
}

