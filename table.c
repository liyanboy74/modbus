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


void table_write(uint16_t *TBALE,uint16_t Index,uint16_t Value)
{
    TBALE[Index]=Value;
}

uint16_t table_read(uint16_t *TBALE,uint16_t Index)
{
    return TBALE[Index];
}
