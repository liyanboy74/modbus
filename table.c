#include "table.h"

/**
 * MODBUS Data model
 * ┌──────────────────┬─────────────┬─────────┐
 * │ Rrimary tables   │ Object type │ Type of │
 * ╞══════════════════╪═════════════╪═════════╡
 * │ Discretes Input  │ Single bit  │    R    │
 * ├──────────────────┼─────────────┼─────────┤
 * │ Coils            │ Single bit  │   R/W   │
 * ├──────────────────┼─────────────┼─────────┤
 * │ Input Registers  │ 16-bit word │    R    │
 * ├──────────────────┼─────────────┼─────────┤
 * │ Holding Registers│ 16-bit word │   R/W   │
 * └──────────────────┴─────────────┴─────────┘
 */

uint16_t TBALE_Discretes_Input  [TBALE_Discretes_Input_Size];
uint16_t TBALE_Coils            [TBALE_Coils_Size];
uint16_t TBALE_Input_Registers  [TBALE_Input_Registers_Size];
uint16_t TABLE_Holding_Registers[TABLE_Holding_Registers_Size];


void mb_table_write(uint16_t *TBALE,uint16_t Index,uint16_t Value)
{
    TBALE[Index]=Value;
}

uint16_t mb_table_read(uint16_t *TBALE,uint16_t Index)
{
    return TBALE[Index];
}

void mb_table_write_bit(uint16_t *TABLE,uint16_t Bit_Index,uint8_t Bit_Value)
{
    uint16_t temp;
    uint16_t TableIndex=Bit_Index/TABLE_Sel_BitSize;
    uint8_t BitOffset=Bit_Index%TABLE_Sel_BitSize;
    Bit_Value&=1;
    temp=TABLE[TableIndex];
    temp&=(~(1<<BitOffset));
    temp|=(Bit_Value<<BitOffset);
    TABLE[TableIndex]=temp;
}

uint8_t mb_table_read_bit(uint16_t *TABLE,uint16_t Bit_Index)
{
    uint16_t ret;
    ret=TABLE[Bit_Index/TABLE_Sel_BitSize];
    ret=ret>>(Bit_Index%TABLE_Sel_BitSize);
    ret&=1;
    return (uint8_t)ret;
}
