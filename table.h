#ifndef _TABLE_H_
#define _TABLE_H_

#include <stdint.h>

//Discrete Output Coils
#define TBALE_DOC_Size_x16      1
//Discrete Input Contacts
#define TBALE_DIC_Size_x16      1
//Analog Input Registers
#define TBALE_AIR_Size_x16      1
//Analog Output Holding Registers
#define TBALE_AOHR_Size_x16     1

//Discrete Output Coils
extern uint16_t TBALE_DOC[];
//Discrete Input Contacts
extern uint16_t TBALE_DIC[];
//Analog Input Registers
extern uint16_t TBALE_AIR[];
//Analog Output Holding Registers
extern uint16_t TBALE_AOHR[];

void TABLE_Write(uint16_t *Table,uint16_t Index,uint16_t Value);
uint16_t TABLE_Read(uint16_t *Table,uint16_t Index);

#endif //_TABLE_H_
