#include <stdint.h>

#include "table.h"

/**
 * Discrete Output Coils
 * Coil/Register Numbers: 1-9999
 * Data Addresses: 0000-270E
 * Read-Write
 */
uint16_t TBALE_DOC[TBALE_DOC_Size_x16];

/**
 * Discrete Input Contacts
 * Coil/Register Numbers: 10001-19999
 * Data Addresses: 0000-270E
 * Read-Only
 */
uint16_t TBALE_DIC[TBALE_DIC_Size_x16];

/**
 * Analog Input Registers
 * Coil/Register Numbers: 30001-39999
 * Data Addresses: 0000-270E
 * Read-Write
 */
uint16_t TBALE_AIR[TBALE_AIR_Size_x16];

/**
 * Analog Output Holding Registers
 * Coil/Register Numbers: 40001-49999
 * Data Addresses: 0000-270E
 * Read-Write
 */
uint16_t TBALE_AOHR[TBALE_AOHR_Size_x16];


void TABLE_Write(uint16_t *Table,uint16_t Index,uint16_t Value)
{
    Table[Index]=Value;
}

uint16_t TABLE_Read(uint16_t *Table,uint16_t Index)
{
    return Table[Index];
}

