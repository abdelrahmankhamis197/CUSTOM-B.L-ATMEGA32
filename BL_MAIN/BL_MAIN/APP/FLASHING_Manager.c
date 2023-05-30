#define  F_CPU 8000000
#include "util/delay.h"
#include "avr/boot.h"
#include "util/crc16.h"
#include "avr/pgmspace.h"

#include "MEMMAP.h"
#include "STDTYPES.h"
#include "UTILS.h"
#include "UART.h"
#include "EEPROM.h"
#include "FLASHING_Manager.h"



u8 len;
u8 RxBuffer[255] ;
static bool_type bIsRequestReceived = FALSE  ;
static u8 SID;
static u8 ReqLen;
static u8 *pReqData ;

static downloadStates enuDownloadState=waiting_ProgrammingSession;



/* **************************************************************** */
/*
    Function Parameters         : TAKES GLOBAL VARS {POINTER 2 ARRAY"THE BUFFER",AND THE LENGTH OF THE REQUEST
    _____________________________
	Function Description        : this FNC set some variable(THE LENGTH OF SR and the pointer of the buffer during the RXC interrupt ISR
    _____________________________
*/

void flashing_vidRxNotification(u8* pu8Data , u8 u8len)
{
	bIsRequestReceived = TRUE ;
	SID = pu8Data[0]; // the first byte the service ID
	pReqData = &pu8Data[1] ;
	ReqLen = u8len ;
}

/*__________________________________________________________________*/


 /* **************************************************************** */
/*
    Function Parameters         : NONE
    _____________________________
	Function Description        : this FNC IS SUMMING THE DATA RECIEVED FORM UART YOU NEED AT FIRST TO TELL ME HOW MANY BYTES WILL BE SENT
    _____________________________
*/

 void UART_RX_Complete(void)
{
	static u8 RxState = IDLE ;
	static u8 bufferIdx = 0 ;

	if(RxState == IDLE)
	{
		len = UART_ReceiveByteNoBlock(); // refers how many bytes will be sent x10 03 "control session" 2bytes
		                                 // 0x34 + size of data will be transferred and normally the size is page size
										 //0x36 +actual data
										 //0x37 transfer exit
		RxState = RUNNING ;
	}
	else
	{
		RxBuffer[bufferIdx] = UART_ReceiveByteNoBlock();
		bufferIdx ++ ;
		if(bufferIdx == len)
		{
			/*all data is received */
			bufferIdx = 0 ;
			RxState = IDLE ;

			/*call application callback*/
			flashing_vidRxNotification(RxBuffer ,len) ;
		}
	}
}
/*__________________________________________________________________*/


 /* **************************************************************** */
/*
    Function Parameters         : POINTER TO BUFF ARRAY AND THE PAGE NUMBER
    _____________________________
	Function Description        : THIS FNC FOR FLASHING THE HEX FILE THAT WAS SENT BY UART TO ECU
    _____________________________
*/

void boot_program_page (u16 page, u8 *buf)
{
	u16 i;
	u8 sreg;
	u32 address;
	u16 word ;

	// Disable interrupts.
	sreg = SREG;
	cli();
	address = page * SPM_PAGESIZE ;

	boot_page_erase_safe (address);

	for (i=0; i<SPM_PAGESIZE; i+=2)
	{
		// Set up little Endian word.
		word  = *buf++;
		word += (*buf++) << 8;

		boot_page_fill_safe(address + i, word);
	}

	boot_page_write_safe (address);     // Store buffer in flash page.

	// Reenable RWW-section again. We need this if we want to jump back
	// to the application after bootloading.

	boot_rww_enable_safe();

	// Re-enable interrupts (if they were ever enabled).

	SREG = sreg;
}
/*__________________________________________________________________*/


 /* **************************************************************** */
/*
    Function Parameters         : NONE
    _____________________________
	Function Description        : SENDS NEGATIVE RESPOND 
    _____________________________
*/

void LOC_vidSendNegResp(void)
{
	/*TODO : handle NRCs*/
	UART_SendUsingPooling(0x7F) ;
}
/*__________________________________________________________________*/


 /* **************************************************************** */
/*
    Function Parameters         : NONE
    _____________________________
	Function Description        : SENDS POSITIVE RESPOND  BY ADDING 0X40 FOR THE SERVICE ID
    _____________________________
*/

void LOC_vidSendPosResp(void)
{
	UART_SendUsingPooling(SID + 0x40);
}

/*__________________________________________________________________*/


/* **************************************************************** */
/*
    Function Parameters         : THE SECTION (BOOTLD OR APP)
    _____________________________
	Function Description        : TRANSFERS THE VECTOR TABLE TO THE CHOSEN SECTION
    _____________________________
*/

void APP_vidMoveIVT(u8 u8Section)
{
	/*Move IVt to Application [starting from 0]*/
	if(u8Section == APP_SECTION)
	{
		/* Enable change of interrupt vectors */
		GICR = (1<<IVCE);
		/* Move interrupts to Application section */
		CLR_BIT(GICR,IVSEL);
	}

	/*Move IVt to Bootloader [starting after end of app section]*/
	else
	{
		/* Enable change of interrupt vectors */
		GICR = (1<<IVCE);
		/* Move interrupts to boot Flash section */
		GICR = (1<<IVSEL);
	}
}
/*__________________________________________________________________*/



/* **************************************************************** */
/*
    Function Parameters         : THE SECTION (BOOTLD OR APP)
    _____________________________
	Function Description        : TRANSFERS THE VECTOR TABLE TO THE CHOSEN SECTION
    _____________________________
*/
void flashingMngr_vidMainTask(void)
{
	static u16 u16CodeSize , u16ReceivedLen = 0 ;
	static u8 u8PageNo = 0 ;
	bool_type bValidReq = FALSE, bValidCRC ;
	u16 u16ReceivedCRC ;

	if(bIsRequestReceived)
	{
		switch(SID)
		{
			case SESSION_CONTROL:
			{
				if(pReqData[0] == PROGRAMMING_SESSION && (ReqLen == 2) &&
				(enuDownloadState ==waiting_ProgrammingSession ))
				{
					/*Programming session, Valid request*/
					LOC_vidSendPosResp();
					enuDownloadState = waiting_DownloadRequest ;
				}
				else
				{
					/*Invalid request, or sequence error*/
					/*Reset download state*/
					enuDownloadState = waiting_ProgrammingSession ;
					LOC_vidSendNegResp();
					/*TODO: specify NRC*/
				}
			}break;

			case DOWNLOAD_REQUEST:
			{
				if((enuDownloadState == waiting_DownloadRequest) &&
				(ReqLen == 3))
				{
					/*Valid request*/
					/*Big endian 2 bytes code size*/
					u16CodeSize = pReqData[0]<<8 | pReqData[1] ;
					if(u16CodeSize < MAX_CODE_SIZE)
					{
						LOC_vidSendPosResp();
						enuDownloadState = waiting_TransferData ;
						bValidReq = TRUE ;
					}
				}
				if (bValidReq != TRUE)
				{
					/*Invalid request, or sequence error*/
					/*Reset download state*/
					enuDownloadState = waiting_ProgrammingSession ;
					LOC_vidSendNegResp();
					/*TODO: specify NRC*/
				}
			}break;

			case TRANSFER_DATA:
			{
				if((enuDownloadState ==waiting_TransferData) &&
				(ReqLen == PAGE_SIZE+1))
				{
					/*Write flash page, starting from index 1 [neglect SID] */
					boot_program_page(u8PageNo ,&pReqData[0]);
					LOC_vidSendPosResp();
					u8PageNo ++ ;
					u16ReceivedLen += PAGE_SIZE ;
					if(u16ReceivedLen == u16CodeSize)
					{
						/*Download is Done*/
						enuDownloadState = waiting_TransferExit;
					}
					else
					{
						/*Download is in progress .. */
					}
				}
				else
				{
					/*Invalid request, or sequence error*/
					/*Reset download state*/
					enuDownloadState = waiting_ProgrammingSession ;
					LOC_vidSendNegResp();
					/*TODO: specify NRC*/
				}
			}break;
			case TRANSFER_EXIT:
			{
				if((enuDownloadState == waiting_TransferExit) &&
				(ReqLen == 1))
				{
					LOC_vidSendPosResp();
					enuDownloadState = waiting_CheckCRC;
				}
				else
				{
					/*Invalid request, or sequence error*/
					/*Reset download state*/
					enuDownloadState = waiting_ProgrammingSession ;
					LOC_vidSendNegResp();
					/*TODO: specify NRC*/
				}
			}break;
			case CHECK_CRC:
			{
				if((enuDownloadState == waiting_CheckCRC) &&
				(ReqLen == 3))
				{
					u16ReceivedCRC = pReqData[0]<<8 | pReqData[1] ;
					/*Validate CRC of flashed code, in range of 0~codeSize*/
					bValidCRC = LOC_vidCheckFlashCRC(0, u16CodeSize, u16ReceivedCRC);
					if(bValidCRC)
					{
						LOC_vidSendPosResp();
						/*Mark application as Valid*/
						EEPROM_WriteData (VALID_APP_ADDRESS,EEPROM_BYTE_SET);   /////////////////////////////
						#if USE_INTERRUPT ==1
						/*Move vector Table to Application section*/
						APP_vidMoveIVT(APP_SECTION);
						#endif
						_delay_ms(100) ;
						/*start the actual program*/
						asm("jmp 0");
					}
					else
					{
						/*Invalid request, or sequence error*/
						LOC_vidSendNegResp();
						/*TODO: specify NRC*/
					}
				}
				else
				{
					/*Invalid request, or sequence error*/
					LOC_vidSendNegResp();
					/*TODO: specify NRC*/
				}

				enuDownloadState = waiting_ProgrammingSession;
			}break;

			default:
			{
				/*Unknown SID*/
				/*Invalid request, or sequence error*/
				/*Reset download state*/
				enuDownloadState = waiting_ProgrammingSession ;
				LOC_vidSendNegResp();
				/*TODO: specify NRC*/
			}
		}
		bIsRequestReceived = FALSE ;
	}
}

/*__________________________________________________________________*/



u8 LOC_vidCheckFlashCRC(u16 u16StartAdd, u16 u16EndAdd, u16 u16CRC)
{
	u16 addr;
	u8  u8Byte;
	u16 CRC16 = 0xFFFF ;

	/* Compute the CRC */
	for(addr = u16StartAdd; addr < u16EndAdd; addr++)
	{
		u8Byte = pgm_read_byte(addr);
		CRC16 = _crc16_update(CRC16, u8Byte);
	}

	/*Compare calculated CRC with received one*/
	if(u16CRC != CRC16 )
	{
		return 0; /* Bad CRC */
	}
	else
	{
		return 1 ; /* Good CRC */
	}
}

void flashingMngr_vidHandleReqFromApp(void)
{
	/*Programming session has been sent to Application [Reprogramming request]*/
	SID = 0x10 ;
	LOC_vidSendPosResp();
	enuDownloadState = waiting_DownloadRequest ;
}