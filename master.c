#include "master.h"
#include "mb.h"
#include "table.h"

mb_error_e mb_check_func_supported(uint8_t func)
{
    switch (func)
    {
        case MB_Read_Coils:break;
        case MB_Read_Discrete_Inputs:break;
        case MB_Read_Holding_Registers:break;
        case MB_Read_Input_Registers:break;
        case MB_Write_Single_Coil:break;
        case MB_Write_Single_Register:break;
        case MB_Write_Multiple_Coils:break;
        case MB_Write_Multiple_Registers:break;
        default:return MB_ERROR_ILLEGAL_FUNCTION;
    }
    return MB_OK;
}

mb_error_e mb_check_quantity(uint16_t Quantity)
{
    if(Quantity>=0x0001&&Quantity<=0x07d0)return MB_OK;
    return MB_ERROR_ILLEGAL_DATA_VALUE;
}

mb_error_e mb_check_bit_address(uint16_t StartAd,uint16_t Quantity)
{
    if(StartAd>=(TBALE_Coils_Size*TABLE_Sel_BitSize))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    if((StartAd+Quantity)>=(TBALE_Coils_Size*TABLE_Sel_BitSize))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    return MB_OK;
}

