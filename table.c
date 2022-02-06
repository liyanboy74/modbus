#include <stdint.h>

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


void TABLE_Write(uint16_t *Table,uint16_t Index,uint16_t Value)
{
    Table[Index]=Value;
}

uint16_t TABLE_Read(uint16_t *Table,uint16_t Index)
{
    return Table[Index];
}
