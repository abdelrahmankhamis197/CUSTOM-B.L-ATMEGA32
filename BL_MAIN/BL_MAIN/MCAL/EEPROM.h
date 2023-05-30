
#ifndef EEPROM_H_
#define EEPROM_H_


void EEPROM_WriteData(u16 add,u8 data);
u8 EEPROM_ReadData(u16 add);

#endif /* EEPROM_H_ */