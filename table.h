#ifndef _TABLE_H_
#define _TABLE_H_

#include <stdint.h>

#define TBALE_Discretes_Input_Size      1
#define TBALE_Coils_Size                1
#define TBALE_Input_Registers_Size      1
#define TABLE_Holding_Registers_Size    1

#define TABLE_Sel_BitSize               16

extern uint16_t TBALE_Discretes_Input  [];
extern uint16_t TBALE_Coils            [];
extern uint16_t TBALE_Input_Registers  [];
extern uint16_t TABLE_Holding_Registers[];

void mb_table_write(uint16_t *Table,uint16_t Index,uint16_t Value);
uint16_t mb_table_read(uint16_t *Table,uint16_t Index);

void mb_table_write_bit(uint16_t *TABLE,uint16_t Bit_Index,uint8_t Bit_Value);
uint8_t mb_table_read_bit(uint16_t *TABLE,uint16_t Bit_Index);

#endif //_TABLE_H_