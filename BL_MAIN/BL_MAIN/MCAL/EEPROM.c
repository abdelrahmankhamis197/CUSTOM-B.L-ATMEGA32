#include "MEMMAP.h"
#include "UTILS.h"
#include "STDTYPES.h"
#include "EEPROM.h"

void EEPROM_WriteData(u16 add,u8 data)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address and data registers */
	EEAR = add;
	EEDR = data;
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
}

u8 EEPROM_ReadData(u16 add)
{
	/* Wait for completion of previous Read */
	while(EECR & (1<<EEWE));
	/* Set up address register */
	EEAR = add;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from data register */
	return EEDR;
}