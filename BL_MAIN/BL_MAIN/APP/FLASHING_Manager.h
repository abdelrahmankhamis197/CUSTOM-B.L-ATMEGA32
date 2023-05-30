
#ifndef FLASHING_MANAGER_H_
#define FLASHING_MANAGER_H_

//------------------ UDS SIDs-------------------
#define SESSION_CONTROL			(0X10)
#define PROGRAMMING_SESSION  	(0x03)
#define DOWNLOAD_REQUEST		(0X34)
#define TRANSFER_DATA			(0X36)
#define TRANSFER_EXIT			(0X37)
#define CHECK_CRC				(0X31)
/**********************************************/

#define MAX_CODE_SIZE				(0x3800)
#define PAGE_SIZE					(128) /*64 word   word=2bytes*/

/* for handelling rxc isr    */
#define IDLE        0
#define RUNNING     1
/*****************************/

/* for the vector table moving fnc  */
#define APP_SECTION	0
#define BLD_SECTION	1
/*****************************/

#define VALID_APP_ADDRESS	     0x00
#define REQ_FROM_APP_ADDRESS	 0x01
#define EEPROM_BYTE_NOT_SET  0XFF
#define EEPROM_BYTE_SET  0X00

#define USE_INTERRUPT	1

#define  SREG (*(volatile unsigned char*)0x5F)

typedef enum {
	waiting_ProgrammingSession,
	waiting_DownloadRequest,
	waiting_TransferData,
	waiting_TransferExit,
	waiting_CheckCRC
}downloadStates ;

extern void UART_RX_Complete(void);

void flashingMngr_vidMainTask(void);
void flashingMngr_vidHandleReqFromApp(void);
u8 LOC_vidCheckFlashCRC(u16 u16StartAdd, u16 u16EndAdd, u16 u16CRC);
void APP_vidMoveIVT(u8 u8Section);

#endif /* FLASHING_MANAGER_H_ */